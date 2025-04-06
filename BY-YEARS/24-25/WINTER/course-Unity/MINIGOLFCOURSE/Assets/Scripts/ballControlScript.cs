using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using EZCameraShake;

public class NewBehaviourScript : MonoBehaviour
{
    public float maxPower;
    public float lineLength;
    public Slider powerSlider;
    public TextMeshProUGUI puttCountLabel;
    public float minHoleTime;
    public AudioClip puttSound;
    public AudioClip holeSound;
    public AudioClip outOffBoundsSound;
    public GameObject completeLevelUI;

    private Rigidbody ball;
    private LineRenderer line;
    private float angle;
    private float powerUpTime;
    private float power;
    private int puttCount;
    private float HoleTime;
    private Vector3 lastPlayerPos;
    private AudioSource audioSource;

    private void Awake()
    {
        ball = GetComponent<Rigidbody>();
        ball.maxAngularVelocity = 1042;
        line = GetComponent<LineRenderer>();
        audioSource = GetComponent<AudioSource>();
    }

    private void Update()
    {
        if (ball.velocity.magnitude < 0.01f)
        {
            if (Input.GetKeyUp(KeyCode.Space))
            {
                Putt();
            }
            if (Input.GetKey(KeyCode.Space))
            {
                PowerUp();
            }

            UpdateLinePosition();
        }
        else
        {
            line.enabled = false;
        }
    }

    private void UpdateLinePosition()
    {
        if (HoleTime == 0)
        {
            line.enabled = true;
        }
        // basicly tutaj ogarniamy mysz
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

        Plane groundPlane = new Plane(Vector3.up, transform.position);

        if (groundPlane.Raycast(ray, out float distance))
        {
            Vector3 worldMousePosition = ray.GetPoint(distance);

            Vector3 direction = (worldMousePosition - transform.position).normalized;

            line.SetPosition(0, transform.position); // od kulki
            line.SetPosition(1, transform.position + direction * lineLength); // na ustalana odleglosc

            angle = Mathf.Atan2(direction.z, direction.x) * Mathf.Rad2Deg;
        }
    }


    private void Putt()
    {
        // camera shake dodany

        CameraShaker.Instance.ShakeOnce(4f, 4f, 0.1f, 1f);
        

        audioSource.PlayOneShot(puttSound); // audio
        lastPlayerPos = transform.position;

        // kierunek gdzie uderzymy pilke 
        Vector3 direction = new Vector3(Mathf.Cos(angle * Mathf.Deg2Rad), 0, Mathf.Sin(angle * Mathf.Deg2Rad));

        // uderzamy pilke
        ball.AddForce(direction * maxPower * power, ForceMode.Impulse);

        powerUpTime = 0;
        powerSlider.value = 0;
        puttCount++;
        puttCountLabel.text = puttCount.ToString();
    }

    private void PowerUp()
    { // w zasadzie jak mocno uderzymy i ogarniamy sile udezenia (wyswietlanie jej)
        powerUpTime += Time.deltaTime; 
        power = Mathf.PingPong(powerUpTime, 1); // (0, 1)
        powerSlider.value = power; 
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.tag == "HOLE")
        {
            CountHoleTime();
        }
    }

    private bool isTransitioning = false; 

    private void CountHoleTime()
    {
        HoleTime += Time.deltaTime;
        if (HoleTime >= minHoleTime && !isTransitioning)
        {
            isTransitioning = true; 
            StartCoroutine(LoadNextSceneWithDelay());
            HoleTime = 0;
        }
    }


    private IEnumerator LoadNextSceneWithDelay()
    {
        completeLevelUI.SetActive(true);

        
        audioSource.clip = holeSound;
        audioSource.volume = 0.5f;
        audioSource.Play();

        // czekaj az sie skonczy dzwiek
        yield return new WaitForSeconds(holeSound.length);

        
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }



    private void OnTriggerExit(Collider other)
    {
        if (other.tag == "HOLE")
        {
            hasPlayedHoleSound = false; 
        }
    }

    private bool hasPlayedHoleSound = false;

    private void OnTriggerEnter(Collider other)
    {
        if (other.tag == "HOLE" && !hasPlayedHoleSound)
        {
            hasPlayedHoleSound = true;

            
            audioSource.PlayOneShot(holeSound, 0.5f);

            
            ball.velocity = Vector3.zero;
            ball.angularVelocity = Vector3.zero;
        }
    }



    private void OnCollisionEnter(Collision collision)
    {
        if (collision.collider.tag == "outOfBounds") 
        {
            
            transform.position = lastPlayerPos;
            ball.velocity = Vector3.zero;
            ball.angularVelocity = Vector3.zero;

            
            audioSource.PlayOneShot(outOffBoundsSound, 0.5f);
        }
    }
}

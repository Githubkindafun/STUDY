using UnityEngine;


public class CameraController : MonoBehaviour
{
    public float rotationSpeed = 100f;
    public float movementSpeed = 10f;
    public float maxPitch = 80f;

    private float yaw = 0f;
    private float pitch = 0f;
    private Vector3 initialPosition;
    private Quaternion initialRotation;

    void Start()
    {
        
        initialPosition = transform.position;
        initialRotation = transform.rotation;

        Vector3 eulerAngles = transform.rotation.eulerAngles;
        pitch = eulerAngles.x;
        yaw = eulerAngles.y;

        Debug.Log("Camera initial position: " + initialPosition);
        Debug.Log("Camera initial rotation: " + eulerAngles);
    }

    void Update()
    {
        HandleRotation();
        HandleMovement();

        if (Input.GetKeyDown(KeyCode.R))
        {
            ResetCamera();
        }
    }

    private void HandleRotation()
    {
        if (Input.GetKey(KeyCode.W))
        {
            pitch -= rotationSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.S))
        {
            pitch += rotationSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.A))
        {
            yaw -= rotationSpeed * Time.deltaTime;
        }
        if (Input.GetKey(KeyCode.D))
        {
            yaw += rotationSpeed * Time.deltaTime;
        }

        
        pitch = Mathf.Clamp(pitch, -maxPitch, maxPitch);

        
        transform.rotation = Quaternion.Euler(pitch, yaw, transform.rotation.eulerAngles.z);
    }

    private void HandleMovement()
    {
        Vector3 direction = Vector3.zero;

        if (Input.GetKey(KeyCode.UpArrow))
        {
            direction += transform.forward;
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
            direction -= transform.forward;
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            direction -= transform.right;
        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
            direction += transform.right;
        }

        direction.Normalize();

        transform.position += direction * movementSpeed * Time.deltaTime;
    }

    private void ResetCamera()
    {
        
        transform.position = initialPosition;
        transform.rotation = initialRotation;

        Vector3 eulerAngles = initialRotation.eulerAngles;
        yaw = eulerAngles.y;
        pitch = eulerAngles.x;

        Debug.Log("Camera reset to initial position: " + initialPosition);
        Debug.Log("Camera reset to initial rotation: " + eulerAngles);
    }
}

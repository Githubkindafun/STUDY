using UnityEngine;

public class MusicManager : MonoBehaviour
{
    private static MusicManager instance;

    void Awake()
    {
        // Sprawd�, czy instancja ju� istnieje
        if (instance != null && instance != this)
        {
            Destroy(gameObject); // Usu� duplikaty
            return;
        }

        // Zachowaj t� instancj�
        instance = this;
        DontDestroyOnLoad(gameObject);
    }
}

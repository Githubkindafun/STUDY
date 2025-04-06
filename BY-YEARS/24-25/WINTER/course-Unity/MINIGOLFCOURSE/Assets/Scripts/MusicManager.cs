using UnityEngine;

public class MusicManager : MonoBehaviour
{
    private static MusicManager instance;

    void Awake()
    {
        // SprawdŸ, czy instancja ju¿ istnieje
        if (instance != null && instance != this)
        {
            Destroy(gameObject); // Usuñ duplikaty
            return;
        }

        // Zachowaj tê instancjê
        instance = this;
        DontDestroyOnLoad(gameObject);
    }
}

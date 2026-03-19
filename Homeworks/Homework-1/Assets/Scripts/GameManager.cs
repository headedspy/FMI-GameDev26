using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    [SerializeField] private int keysToWin = 3;

    private int keysCollected = 0;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        Instance = this;
        DontDestroyOnLoad(gameObject);
    }

    private void Start()
    {
        KeysHUDManager.Instance.Init(keysToWin);
    }

    public void OnKeyCollected()
    {
        keysCollected++;

        if (keysCollected >= keysToWin)
        {
            WinGame();
        }
    }

    public void OnPlayerDamaged()
    {
        int currentHealth = HealthManager.Instance.GetCurrentHealth();

        if (currentHealth <= 0)
        {
            LoseGame();
        }
    }

    private void WinGame()
    {
        Debug.Log("YOU WIN!");
        // TODO: SceneManager.LoadScene("WinScene");
        // TODO: Show win UI panel
    }

    private void LoseGame()
    {
        Debug.Log("YOU LOSE!");
        Time.timeScale = 0;
        // TODO: SceneManager.LoadScene("LoseScene");
        // TODO: Show lose UI / restart prompt
    }

    public void ResetGame()
    {
        keysCollected = 0;
    }
}
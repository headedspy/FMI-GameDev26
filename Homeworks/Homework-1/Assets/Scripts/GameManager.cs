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
        SceneManager.LoadScene("WinGame");
    }

    private void LoseGame()
    {
        SceneManager.LoadScene("LoseGame");
    }

    public void ResetGame()
    {
        keysCollected = 0;
    }
}
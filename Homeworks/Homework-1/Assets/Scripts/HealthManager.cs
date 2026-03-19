using UnityEngine;
using UnityEngine.UI;

public class HealthManager : MonoBehaviour
{
    public static HealthManager Instance;

    [Header("Health")]
    public int maxHealth = 3;
    private int currentHealth;

    [Header("Damage Cooldown")]
    public float damageCooldown = 1f;
    private float cooldownTimer = 0f;

    [Header("Heart UI")]
    public Image[] heartImages;
    public Sprite fullHeartSprite;
    public Sprite emptyHeartSprite;

    CustomAnimator ca;

    void Awake()
    {
        Instance = this;
    }

    void Start()
    {
        ca = GetComponent<CustomAnimator>();
        currentHealth = maxHealth;
        UpdateHeartsUI();
    }

    void Update()
    {
        if (cooldownTimer > 0f)
        {
            cooldownTimer -= Time.deltaTime;
        }
    }

    public void TakeDamage(int amount)
    {
        if (currentHealth <= 0) return;
        if (cooldownTimer > 0f) return;  // still in invincibility window

        currentHealth -= amount;
        currentHealth = Mathf.Clamp(currentHealth, 0, maxHealth);
        cooldownTimer = damageCooldown;

        GameManager.Instance.OnPlayerDamaged();
        ScreenEffectsController.Instance.TriggerHit();
        ca.TriggerHurt();

        UpdateHeartsUI();
        CheckLowHealth();

    }

    public int GetCurrentHealth()
    {
        return currentHealth;
    }

    public void HealFull()
    {
        currentHealth = maxHealth;

        UpdateHeartsUI();
        CheckLowHealth();
    }

    private void CheckLowHealth()
    {
        ScreenEffectsController.Instance.SetLowHealth(currentHealth == 1);
    }

    private void UpdateHeartsUI()
    {
        for (int i = 0; i < heartImages.Length; i++)
        {
            heartImages[i].sprite = i < currentHealth ? fullHeartSprite : emptyHeartSprite;
        }
    }
}
using System.Collections;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class LightningBolt : MonoBehaviour
{
    public float duration = 1f;
    public float cooldown = 2f;
    public int uses = 3;
    public TextMeshProUGUI usesText;
    public Slider cooldownSlider;
    private GameObject lightningBolt;
    private bool onCooldown = false;

    void Start()
    {
        lightningBolt = transform.Find("LightningBolt").gameObject;
        lightningBolt.SetActive(false);
        UpdateUsesText();
        cooldownSlider.minValue = 0;
        cooldownSlider.maxValue = 1;
        cooldownSlider.value = 1;
    }

    void Update()
    {
        if (Input.GetButtonDown("Fire1") && !onCooldown && uses > 0)
        {
            if(GetComponent<Jump>().GetIsOnGround() == false)
            {
                StartCoroutine(ShowBolt());
            }
        }
    }

    IEnumerator ShowBolt()
    {
        uses--;
        UpdateUsesText();
        onCooldown = true;
        lightningBolt.SetActive(true);
        yield return new WaitForSeconds(duration);
        lightningBolt.SetActive(false);

        // Tick down the slider over the cooldown duration
        float elapsed = 0f;
        while (elapsed < cooldown)
        {
            elapsed += Time.deltaTime;
            cooldownSlider.value = elapsed / cooldown;
            yield return null;
        }

        cooldownSlider.value = 1;
        onCooldown = false;
    }

    public void AddUse()
    {
        uses++;
        UpdateUsesText();
    }

    void UpdateUsesText()
    {
        usesText.text = uses.ToString();
    }
}
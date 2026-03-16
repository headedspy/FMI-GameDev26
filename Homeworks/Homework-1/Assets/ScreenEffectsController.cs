using System.Collections;
using UnityEngine;
using UnityEngine.Rendering.PostProcessing;

public class ScreenEffectsController : MonoBehaviour
{
    public static ScreenEffectsController Instance;

    [Header("Post Process Volume")]
    public PostProcessVolume volume;

    private Vignette vignette;
    private LensDistortion lensDistortion;

    [Header("Vignette")]
    public float vignetteIntensity = 0.55f;
    public float vignettePulseSpeed = 2f;

    [Header("Shake")]
    public float shakeIntensity = 30f;
    public float shakeDuration = 0.4f;

    private bool isLowHealth = false;
    private float pulseTimer = 0f;

    void Awake()
    {
        Instance = this;

        if (volume == null)
        {
            Debug.LogError("ScreenEffectsController: No PostProcessVolume assigned!");
            return;
        }

        if (!volume.profile.TryGetSettings(out vignette))
            Debug.LogWarning("ScreenEffectsController: Vignette not found in profile!");

        if (!volume.profile.TryGetSettings(out lensDistortion))
            Debug.LogWarning("ScreenEffectsController: LensDistortion not found in profile!");
    }

    void Update()
    {
        if (!isLowHealth || vignette == null) return;

        pulseTimer += Time.deltaTime * vignettePulseSpeed;
        vignette.intensity.value = Mathf.Lerp(0.3f, vignetteIntensity, (Mathf.Sin(pulseTimer) + 1f) / 2f);
    }

    public void SetLowHealth(bool isLow)
    {
        isLowHealth = isLow;

        if (vignette == null) return;

        vignette.enabled.value = isLow;
        vignette.color.value = Color.red;

        if (isLow)
        {
            StartCoroutine(ShakeRoutine());
        }
        else
        {
            vignette.intensity.value = 0f;
            pulseTimer = 0f;
        }
    }

    public void TriggerHit()
    {
        StartCoroutine(ShakeRoutine());
    }

    private IEnumerator ShakeRoutine()
    {
        if (lensDistortion == null) yield break;

        float elapsed = 0f;
        while (elapsed < shakeDuration)
        {
            float strength = Mathf.Lerp(shakeIntensity, 0f, elapsed / shakeDuration);
            lensDistortion.intensity.value = Random.Range(-strength, strength);
            elapsed += Time.deltaTime;
            yield return null;
        }

        lensDistortion.intensity.value = 0f;
    }
}
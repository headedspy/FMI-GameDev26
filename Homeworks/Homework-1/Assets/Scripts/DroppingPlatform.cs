using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DroppingPlatform : MonoBehaviour
{
    [Header("Shake Settings")]
    [SerializeField] float shakeDuration = 0.5f;
    [SerializeField] float shakeMagnitude = 0.05f;

    Vector3 startPosition;
    bool isTriggered = false;

    Rigidbody2D rb;

    void Start()
    {
        startPosition = transform.position;
        rb = GetComponent<Rigidbody2D>();

        // static until triggered
        rb.bodyType = RigidbodyType2D.Static;
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player" && !isTriggered)
        {
            isTriggered = true;
            StartCoroutine(ShakeAndDrop());
        }
    }

    IEnumerator ShakeAndDrop()
    {
        float elapsed = 0f;
        while (elapsed < shakeDuration)
        {
            float offsetX = Random.Range(-shakeMagnitude, shakeMagnitude);
            float offsetY = Random.Range(-shakeMagnitude, shakeMagnitude);
            transform.position = startPosition + new Vector3(offsetX, offsetY, 0);

            elapsed += Time.deltaTime;
            yield return null;
        }

        // Snap back before dropping
        transform.position = startPosition;

        gameObject.layer = LayerMask.NameToLayer("Falling");

        rb.bodyType = RigidbodyType2D.Dynamic;
    }
}
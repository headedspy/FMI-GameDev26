using System.Collections;
using UnityEngine;

public class DroppingPlatform : MonoBehaviour
{
    [Header("Shake Settings")]
    [SerializeField] float shakeDuration = 0.5f;
    [SerializeField] float shakeMagnitude = 0.05f;

    [Header("Respawn Settings")]
    [SerializeField] float respawnDelay = 3f;

    Vector3 startPosition;
    bool isTriggered = false;
    Rigidbody2D rb;
    Collider2D col;

    void Start()
    {
        startPosition = transform.position;
        rb = GetComponent<Rigidbody2D>();
        col = GetComponent<Collider2D>();
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
        // Shake
        float elapsed = 0f;
        while (elapsed < shakeDuration)
        {
            float offsetX = Random.Range(-shakeMagnitude, shakeMagnitude);
            float offsetY = Random.Range(-shakeMagnitude, shakeMagnitude);
            transform.position = startPosition + new Vector3(offsetX, offsetY, 0);
            elapsed += Time.deltaTime;
            yield return null;
        }

        // Drop and phase through everything
        transform.position = startPosition;
        col.isTrigger = true;
        gameObject.layer = LayerMask.NameToLayer("Falling");
        rb.bodyType = RigidbodyType2D.Dynamic;

        yield return new WaitForSeconds(respawnDelay);

        Respawn();
    }

    void Respawn()
    {
        rb.velocity = Vector2.zero;
        rb.bodyType = RigidbodyType2D.Static;
        col.isTrigger = false;
        gameObject.layer = LayerMask.NameToLayer("Default");
        transform.position = startPosition;
        isTriggered = false;
    }
}
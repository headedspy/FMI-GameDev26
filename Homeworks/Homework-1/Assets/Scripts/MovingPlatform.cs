using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingPlatform : MonoBehaviour
{
    [SerializeField] float speed = 4f;
    [SerializeField] float pauseDuration = 2f;
    [SerializeField] bool moveVertically = false;

    float direction = -1f;
    bool isPausing = false;

    void Update()
    {
        if (isPausing) return;

        Vector2 move = moveVertically ? new Vector2(0, direction * speed) : new Vector2(direction * speed, 0);
        transform.Translate(move * Time.deltaTime);
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player")
        {
            collision.transform.SetParent(transform);
        }
    }

    void OnCollisionExit2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player")
        {
            collision.transform.SetParent(null);
        }
    }

    void OnTriggerEnter2D(Collider2D col)
    {
        if (!isPausing)
        {
            StartCoroutine(PauseAndFlip());
        }
    }

    IEnumerator PauseAndFlip()
    {
        isPausing = true;
        yield return new WaitForSeconds(pauseDuration);
        direction *= -1;
        isPausing = false;
    }
}
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JumpPad : MonoBehaviour
{
    [Header("Jump Settings")]
    [SerializeField] float jumpForce = 20f;

    [Header("Timing")]
    [SerializeField] float springInDuration = 0.1f;
    [SerializeField] float springOutDuration = 0.2f;

    [Header("Sprites")]
    [SerializeField] Sprite springNormal;
    [SerializeField] Sprite springIn;
    [SerializeField] Sprite springOut;

    [Header("Squish")]
    [SerializeField] float squishDownAmount = 0.15f;
    [SerializeField] float squishUpAmount = 0.15f;

    SpriteRenderer sr;
    Vector3 startPosition;
    bool isAnimating = false;

    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        sr.sprite = springNormal;
        startPosition = transform.position;
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player" && !isAnimating)
        {
            foreach (ContactPoint2D contact in collision.contacts)
            {
                if (contact.normal.y < -0.5f)
                {
                    Rigidbody2D playerRb = collision.gameObject.GetComponent<Rigidbody2D>();
                    Jump playerJump = playerRb.GetComponent<Jump>();
                    if (playerJump != null) playerJump.Launch();

                    StartCoroutine(SpringAnimation(playerRb, playerJump));
                    break;
                }
            }
        }
    }

    IEnumerator SpringAnimation(Rigidbody2D playerRb, Jump playerJump)
    {
        isAnimating = true;

        sr.sprite = springIn;
        transform.position = startPosition + Vector3.down * squishDownAmount;
        yield return new WaitForSeconds(springInDuration);

        playerRb.velocity = new Vector2(playerRb.velocity.x, jumpForce);

        sr.sprite = springOut;
        transform.position = startPosition + Vector3.up * squishUpAmount;
        yield return new WaitForSeconds(springOutDuration);

        sr.sprite = springNormal;
        transform.position = startPosition;
        isAnimating = false;
    }
}
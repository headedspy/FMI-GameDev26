using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jump : MonoBehaviour
{
    [Header("Jump Settings")]
    [SerializeField] float jumpForce = 8f;
    [SerializeField] float riseMultiplier = 2f;
    [SerializeField] float fallMultiplier = 3.5f;
    [SerializeField] float lowJumpMultiplier = 2f;
    [SerializeField] float coyoteTime = 0.12f;

    bool isOnGround = false;
    float coyoteTimer = 0f;
    bool isLaunched = false;

    Rigidbody2D rb;
    CustomAnimator ca;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        ca = GetComponent<CustomAnimator>();
    }

    void Update()
    {
        coyoteTimer -= Time.deltaTime;

        if (!isLaunched && isOnGround)
        {
            coyoteTimer = coyoteTime;
        }

        if (!isLaunched && Input.GetButtonDown("Jump") && coyoteTimer > 0f)
        {
            rb.velocity = new Vector2(rb.velocity.x, jumpForce);
            coyoteTimer = 0f;
        }

        if (isLaunched || !isOnGround)
        {
            ApplyAirborneGravity();
        }

        bool isInAir = !isOnGround;
        ca.SetJumping(isInAir);
    }

    public void Launch()
    {
        isLaunched = true;
    }

    void ApplyAirborneGravity()
    {
        // falling
        if (rb.velocity.y < 0)
        {
            rb.velocity += Vector2.up * Physics2D.gravity.y * (fallMultiplier - 1) * Time.deltaTime;
        }
        // gravity with button released
        else if (rb.velocity.y > 0 && (!Input.GetButton("Jump") || isLaunched))
        {
            rb.velocity += Vector2.up * Physics2D.gravity.y * (lowJumpMultiplier - 1) * Time.deltaTime;
        }
        // gravity with button held
        else if (rb.velocity.y > 0 && Input.GetButton("Jump") && !isLaunched)
        {
            rb.velocity += Vector2.up * Physics2D.gravity.y * (riseMultiplier - 1) * Time.deltaTime;
        }
    }

    void OnCollisionEnter2D(Collision2D collider2D)
    {
        Vector2 boxPosition = transform.position;
        boxPosition.y -= 1.1f;

        RaycastHit2D[] hits = Physics2D.BoxCastAll(boxPosition, new Vector2(1, 1), 0, Vector2.zero);
        isOnGround = false;

        foreach (var hit in hits)
        {
            if (hit.collider.gameObject.name != "Player")
            {
                isOnGround = true;

                if (isLaunched && hit.collider.gameObject.name != "JumpPad")
                {
                    isLaunched = false;
                }
                break;
            }
        }
    }

    void OnCollisionExit2D(Collision2D collider2D)
    {
        isOnGround = false;
    }
}
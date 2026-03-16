using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    [SerializeField] float speed = 8f;
    [SerializeField] float acceleration = 15f;
    [SerializeField] float deceleration = 20f;

    float horizontal;
    Rigidbody2D rb;

    // track speed ourselves, don't read from rb because on landing the physics collision response zeroes out all velocity
    float currentSpeed;

    CustomAnimator ca;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        ca = GetComponent<CustomAnimator>();
    }

    void Update()
    {
        horizontal = Input.GetAxis("Horizontal");

        bool isMoving = Mathf.Abs(rb.velocity.x) > 0.1f;
        ca.SetWalking(isMoving);
    }

    void FixedUpdate()
    {
        float targetSpeed = horizontal * speed;

        float rate = Mathf.Abs(horizontal) > 0.01f ? acceleration : deceleration;

        currentSpeed = Mathf.MoveTowards(currentSpeed, targetSpeed, rate * Time.fixedDeltaTime);

        rb.velocity = new Vector2(currentSpeed, rb.velocity.y);
    }
}
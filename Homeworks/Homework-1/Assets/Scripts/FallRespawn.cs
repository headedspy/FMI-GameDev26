using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FallRespawn : MonoBehaviour
{
    [SerializeField] float fallThreshold = -10.0f;
    [SerializeField] Transform respawnPoint;

    Rigidbody2D rb;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    void Update()
    {
        if (transform.position.y < fallThreshold)
        {
            transform.position = respawnPoint.position;
            rb.velocity = Vector2.zero;

            HealthManager.Instance.HealFull();
        }
    }
}
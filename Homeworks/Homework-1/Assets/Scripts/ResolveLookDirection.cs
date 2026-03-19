using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEngine.Mathf;
using TMPro;

public class ResolveLookDirection : MonoBehaviour
{
    Rigidbody2D rb2d;
    public Canvas hudCanvas;
    private Vector3 originalCanvasScale;

    void Start()
    {
        rb2d = GetComponent<Rigidbody2D>();
        originalCanvasScale = hudCanvas.transform.localScale;
    }

    void Update()
    {
        if (!rb2d) return;
        float horizontal_velocity = rb2d.velocity.x;
        if (Abs(horizontal_velocity) > 0.01f)
        {
            float look_direction = (horizontal_velocity > 0) ? 1 : -1;
            transform.localScale = new Vector3(look_direction, 1, 1);
            hudCanvas.transform.localScale = new Vector3(
                originalCanvasScale.x * look_direction,
                originalCanvasScale.y,
                originalCanvasScale.z
            );
        }
    }
}
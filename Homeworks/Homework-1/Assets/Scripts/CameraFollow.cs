using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    [SerializeField] GameObject player;
    [SerializeField] float smoothTime = 0.08f;
    [SerializeField] bool centerOnPlayer = false;

    private Vector3 offset;
    private Vector3 velocity = Vector3.zero;  // Required by SmoothDamp

    void Start()
    {
        offset = transform.position - player.transform.position;
    }

    void Update()
    {
        Vector3 target;

        if (centerOnPlayer)
        {
            // Center X and Y on player, but keep Z offset so the camera stays back
            target = new Vector3(player.transform.position.x, player.transform.position.y, player.transform.position.z + offset.z);
        }
        else
        {
            target = player.transform.position + offset;
        }

        transform.position = Vector3.SmoothDamp(transform.position, target, ref velocity, smoothTime);
    }
}
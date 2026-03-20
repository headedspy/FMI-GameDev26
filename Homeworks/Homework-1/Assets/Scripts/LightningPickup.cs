using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LightningPickup : MonoBehaviour
{
    void OnTriggerEnter2D(Collider2D col)
    {
        if (col.gameObject.name == "Player")
        {
            col.GetComponent<LightningBolt>().AddUse();
            Destroy(gameObject);
        }
    }
}

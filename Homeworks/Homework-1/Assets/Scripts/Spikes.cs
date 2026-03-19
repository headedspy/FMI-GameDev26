using UnityEngine;

public class Spikes : MonoBehaviour
{
    [Header("Damage")]
    public int damageAmount = 1;

    void OnCollisionEnter2D(Collision2D col)
    {
        if (col.gameObject.name == "Player")
        {
            foreach (ContactPoint2D contact in col.contacts)
            {
                if (contact.normal.y < -0.5f)
                {
                    HealthManager.Instance.TakeDamage(damageAmount);
                }
            }
        }
    }
}
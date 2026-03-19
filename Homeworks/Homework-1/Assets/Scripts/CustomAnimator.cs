using System.Collections;
using UnityEngine;

public class CustomAnimator : MonoBehaviour
{
    private Animator animator;

    public float hurtDuration = 0.4f;

    void Awake()
    {
        animator = GetComponent<Animator>();
    }

    public void SetWalking(bool isWalking)
    {
        animator.SetBool("IsWalking", isWalking);
    }

    public void SetJumping(bool isJumping)
    {
        animator.SetBool("IsJumping", isJumping);
    }

    public void TriggerHurt()
    {
        StartCoroutine(HurtRoutine());
    }

    private IEnumerator HurtRoutine()
    {
        animator.SetBool("IsHurt", true);
        yield return new WaitForSeconds(hurtDuration);
        animator.SetBool("IsHurt", false);
    }
}
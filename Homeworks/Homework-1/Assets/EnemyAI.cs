using UnityEngine;
public class EnemyAI : MonoBehaviour
{
    [Header("Movement")]
    [SerializeField] float moveSpeed = 3f;
    [SerializeField] float detectionRange = 8f;
    [SerializeField] float stopDistance = 0.5f;
    [Header("Jumping")]
    [SerializeField] float jumpForce = 7f;
    [SerializeField] float jumpCooldown = 1.2f;
    [SerializeField] float obstacleCheckDistance = 1.2f;
    [SerializeField] float obstacleCheckHeight = 0.2f;
    [SerializeField] float minJumpHeightDiff = 1.5f;
    [SerializeField] LayerMask groundLayer;
    [SerializeField] LayerMask obstacleLayer;
    [Header("Ground Check")]
    [SerializeField] Transform groundCheck;
    [SerializeField] float groundCheckRadius = 0.15f;
    Rigidbody2D rb;
    CustomAnimator ca;
    Transform player;
    float jumpCooldownTimer;
    bool isGrounded;
    bool wasGrounded;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        ca = GetComponent<CustomAnimator>();
        rb.freezeRotation = true;
        player = GameObject.FindGameObjectWithTag("Player").transform;
    }
    void Update()
    {
        if (player == null) return;

        wasGrounded = isGrounded;
        isGrounded = Physics2D.OverlapCircle(
            groundCheck.position,
            groundCheckRadius,
            groundLayer
        );

        // landed
        if (isGrounded && !wasGrounded)
        {
            ca.SetJumping(false);
        }

        jumpCooldownTimer -= Time.deltaTime;

        float distanceToPlayer = Vector2.Distance(transform.position, player.position);
        if (distanceToPlayer <= detectionRange)
        {
            MoveTowardsPlayer(distanceToPlayer);
            if (isGrounded && jumpCooldownTimer <= 0f)
            {
                TryJump();
            }
        }
        else
        {
            rb.velocity = new Vector2(
                Mathf.MoveTowards(rb.velocity.x, 0f, moveSpeed),
                rb.velocity.y
            );
        }

        ca.SetWalking(isGrounded && Mathf.Abs(rb.velocity.x) > 0.1f);
    }

    void MoveTowardsPlayer(float distanceToPlayer)
    {
        if (distanceToPlayer <= stopDistance)
        {
            rb.velocity = new Vector2(
                Mathf.MoveTowards(rb.velocity.x, 0f, moveSpeed),
                rb.velocity.y
            );
            return;
        }

        float direction = Mathf.Sign(player.position.x - transform.position.x);
        rb.velocity = new Vector2(direction * moveSpeed, rb.velocity.y);
        transform.localScale = new Vector3(direction, 1f, 1f);
    }

    void TryJump()
    {
        float direction = Mathf.Sign(player.position.x - transform.position.x);
        Vector2 rayOrigin = new Vector2(
            transform.position.x,
            groundCheck.position.y + obstacleCheckHeight
        );

        RaycastHit2D obstacleHit = Physics2D.Raycast(
            rayOrigin,
            new Vector2(direction, 0f),
            obstacleCheckDistance,
            obstacleLayer
        );

        float heightDiff = player.position.y - transform.position.y;
        bool playerIsAbove = heightDiff > minJumpHeightDiff;
        bool obstacleAhead = obstacleHit.collider != null;
        if (playerIsAbove || obstacleAhead)
        {
            rb.velocity = new Vector2(rb.velocity.x, jumpForce);
            jumpCooldownTimer = jumpCooldown;
            ca.SetJumping(true);
        }
    }
}
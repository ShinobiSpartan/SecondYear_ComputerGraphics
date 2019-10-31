using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent (typeof(CharacterController))]
[RequireComponent (typeof(Animator))]
public class Player : MonoBehaviour
{
    CharacterController controller = null;
    Animator animator = null;

    // Speed the character moves
    public float speed = 80.0f;
    // Amount of force the character exerts on the other objects
    public float pushPower = 2.0f;

    // Prefab for the spheres that spawn
    public GameObject spherePrefab;

    // Start is called before the first frame update
    void Start()
    {
        // Gets the character controller from the object
        controller = GetComponent<CharacterController>();
        // Gets the character controller from the object
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        // Allows the character to be interacted with
        float vertical = Input.GetAxis("Vertical");
        float horizontal = Input.GetAxis("Horizontal");

        // Moves the charater
        controller.SimpleMove(transform.up * Time.deltaTime);
        transform.Rotate(transform.up, horizontal * speed * Time.deltaTime);
        // Plays the animation when the character moves
        animator.SetFloat("Speed", vertical * speed * Time.deltaTime);
    }

    private void OnControllerColliderHit(ControllerColliderHit hit)
    {
        // Get the rigidbody of the object the character collided with
        Rigidbody body = hit.collider.attachedRigidbody;

        // If it doesn't have a rigidbody or is kinematic ignore it
        if (body == null || body.isKinematic)
            return;
        // Don't move the object if there is not enough force
        if (hit.moveDirection.y < 0.3f)
            return;

        // Calculate the direction the object will move when pushed
        Vector3 pushDir = new Vector3(hit.moveDirection.x, 0, hit.moveDirection.z);
        // Push the other object
        body.velocity = pushDir * pushPower;
    }

    public void SpawnSpheres()
    {
        // Spawns a sphere
        Instantiate(spherePrefab, new Vector3(transform.position.x, transform.position.y, transform.position.z + 2), Quaternion.identity);
    }   
}

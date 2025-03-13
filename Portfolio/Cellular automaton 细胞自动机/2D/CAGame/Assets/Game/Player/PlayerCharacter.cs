using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCharacter : MonoBehaviour
{
    Rigidbody rigid;
    public float speed;

    void Awake()
    {
        rigid = GetComponent<Rigidbody>();
    }

    public void Move(Vector3 force)
    {
        rigid.AddForce(force);
    }

    public void Jump()
    {
        if (Physics.Raycast(transform.position, Vector3.down, 0.7f))
        {
            rigid.AddForce(Vector3.up * 8f, ForceMode.Impulse);
        }
    }

}

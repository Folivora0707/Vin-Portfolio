using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MonoBehaviour
{
    public AudioClip pickupSound;

    public void OnTriggerEnter(Collider other)
    {
        var player = other.GetComponent<PlayerCharacter>();

        if (player)
        {
            gameObject.SetActive(false);
            // AudioSource.PlayClipAtPoint(pickupSound, transform.position);
            // AudioSource.PlayClipAtPoint(pickupSound, new Vector3(1, 1, 1));
            // audioSource.PlayOneShot(pickupSound);
            UnityEngine.SceneManagement.SceneManager.LoadScene("Start");
        }
    }

}

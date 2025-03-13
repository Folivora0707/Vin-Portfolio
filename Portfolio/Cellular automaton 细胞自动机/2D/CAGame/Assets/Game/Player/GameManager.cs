using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public void OnClick_Restart()
    {
        UnityEngine.SceneManagement.SceneManager.LoadScene("Start");
    }
}

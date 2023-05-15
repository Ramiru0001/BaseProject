using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Antman : MonoBehaviour
{
    public Animator m_anim;
    public bool sw;
    public GameObject[] m_sword = new GameObject[2];
    public GameObject[] m_gun = new GameObject[2];
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        m_anim.SetBool("sw", sw);
        int n = sw ? 1 : 0;
        m_gun[n].SetActive(true);
        m_sword[n].SetActive(true);

        m_gun[n^1].SetActive(false);
        m_sword[n^1].SetActive(false);


    }
}

// RandomMapMaker.cs
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

public class RandomMapMaker : MonoBehaviour
{

    [SerializeField]
    private float _width = 50;
    [SerializeField]
    private float _depth = 50;

    [SerializeField]
    private float _maxHeight = 10;

    [SerializeField]
    private bool _isCAMap = false;

    [SerializeField]
    private float _mapSize = 1f;

    public GameObject fxObj;
    //=================================================================================
    //初期化
    //=================================================================================
    private void Awake()
    {
        transform.localScale = new Vector3(_mapSize, _mapSize, _mapSize);

        float[,] map = new float[(int)_width, (int)_depth];

        for (int x = 0; x < _width; x++)
        {
            for (int z = 0; z < _depth; z++)
            {
                map[x, z] = Random.value;
            }
        }

        for (int i = 0; i < 10; i++)
        {
            float[,] newmap = new float[(int)_width, (int)_depth];
            for (int x = 0; x < _width; x++)
            {
                for (int z = 0; z < _depth; z++)
                {
                    int nbsnum = 0;
                    for (int ii = -1; ii < 2; ii++)
                    {
                        for (int jj = -1; jj < 2; jj++)
                        {
                            int neighbour_x = x + ii;
                            int neighbour_z = z + jj;
                            if (!(ii == 0 && jj == 0))
                            {                                     //本身不算邻居   
                                if (neighbour_x < 0 || neighbour_z < 0 || neighbour_x >= _width || neighbour_z >= _depth)
                                    nbsnum++;                     //地图边缘视为存在邻居
                                else if (map[neighbour_x, neighbour_z] > 0.5)
                                    nbsnum++;                     //邻居存在，计数器加一
                            }
                        }
                    }
                    if (map[x, z] > 0.5)
                    {
                        if (nbsnum < 4)
                            newmap[x, z] = 0.1f;
                        else
                            newmap[x, z] = 1;
                    }
                    else
                    {
                        if (nbsnum > 4)
                            newmap[x, z] = 1;
                        else
                            newmap[x, z] = 0.1f;
                    }
                }
            }
            for (int x = 0; x < _width; x++)
            {
                for (int z = 0; z < _depth; z++)
                {
                    if (newmap[x, z] == 0.1f)
                        map[x, z] = newmap[x, z];
                    else
                    {
                        int nbsnum = 0;
                        for (int ii = -1; ii < 2; ii++)
                        {
                            for (int jj = -1; jj < 2; jj++)
                            {
                                int neighbour_x = x + ii;
                                int neighbour_z = z + jj;
                                if (!(ii == 0 && jj == 0))
                                {                                     //本身不算邻居   
                                    if (neighbour_x < 0 || neighbour_z < 0 || neighbour_x >= _width || neighbour_z >= _depth)
                                        nbsnum++;                     //地图边缘视为存在邻居
                                    else if (map[neighbour_x, neighbour_z] > 0.5)
                                        nbsnum++;                     //邻居存在，计数器加一
                                }
                            }
                        }
                        map[x, z] = newmap[x, z] - (8 - nbsnum) * 0.1f;
                    }
                }
            }
        }

        for(int x = 0; x < _width; x++)
        {
            for(int z = 0; z < _depth; z++)
            {
                if (map[x, z] == 0.1f)
                {
                    int nbsnum = 0;
                    for (int ii = -1; ii < 2; ii++)
                    {
                        for (int jj = -1; jj < 2; jj++)
                        {
                            int neighbour_x = x + ii;
                            int neighbour_z = z + jj;
                            if (!(ii == 0 && jj == 0))
                            {                                     //本身不算邻居   
                                if (neighbour_x < 0 || neighbour_z < 0 || neighbour_x >= _width || neighbour_z >= _depth)
                                    nbsnum++;                     //地图边缘视为存在邻居
                                else if (map[neighbour_x, neighbour_z] > 0.1f)
                                    nbsnum++;                     //邻居存在，计数器加一
                            }
                        }
                    }
                    if (nbsnum >= 4)                              //生成宝藏
                    {
                        if (Random.value < 0.05f)
                        {
                            GameObject sphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                            //sphere.isStatic = true;
                            Collider c = sphere.GetComponent<Collider>();
                            c.isTrigger = true;
                            sphere.AddComponent<Door>();
                            sphere.transform.localPosition = new Vector3(x, 1, z);
                            sphere.transform.SetParent(transform);
                            Color color = Color.green;
                            sphere.GetComponent<MeshRenderer>().material.color = color;

                            Renderer rend = sphere.GetComponent<Renderer>();
                            rend.enabled = false;
                            // sphere.layer = LayerMask.NameToLayer("MyLayer");      //隐藏绿球
                            GameObject fxquan = GameObject.Instantiate(fxObj, new Vector3(x, 1, z), Quaternion.identity) as GameObject;
                            fxquan.SetActive(true);
                        }
                    }
                }
            }
        }

        for (int x = 0; x < _width; x++)
        {
            for (int z = 0; z < _depth; z++)
            {

                GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
                cube.isStatic = true;
                cube.transform.localPosition = new Vector3(x, 0, z);
                cube.transform.SetParent(transform);
                float y;
                if (_isCAMap)
                    y = _maxHeight * map[x, z];
                else y = _maxHeight * 0.1f;
                for (int i = 3; i < y; i++)
                {
                    GameObject cube1 = GameObject.CreatePrimitive(PrimitiveType.Cube);
                    cube1.isStatic = true;
                    cube1.transform.localPosition = new Vector3(x, 0, z);
                    cube1.transform.SetParent(transform);
                    cube1.transform.localPosition = new Vector3(cube1.transform.localPosition.x, i-2, cube1.transform.localPosition.z);
                    Color color1 = Color.black;
                    ColorUtility.TryParseHtmlString("#D4500EFF", out color1);
                    cube1.GetComponent<MeshRenderer>().material.color = color1;
                }
                Color color = Color.black;
                if (y > _maxHeight * 0.1f)
                {
                    ColorUtility.TryParseHtmlString("#D4500EFF", out color);
                }
                cube.GetComponent<MeshRenderer>().material.color = color;
            }
        }
    }
}

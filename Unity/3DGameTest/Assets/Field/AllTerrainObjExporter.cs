// Converted from UnityScript to C# at http://www.M2H.nl/files/js_to_c.php - by Mike Hergaarden
// C # manual conversion work by Yun Kyu Choi

using UnityEngine;
using UnityEditor;
using System;
using System.Collections;
using System.IO;
using System.Text;


class ExportAllTerrain : EditorWindow
{
    SaveFormat saveFormat = SaveFormat.Triangles;
    SaveResolution saveResolution = SaveResolution.Half;

    static Terrain[] terrains=null;
    static Vector3[] terrainPoss = null;

    int tCount;
    int counter;
    int totalCount;
    int progressUpdateInterval = 10000;

    [MenuItem("Terrain/AllExport To Obj...")]
    static void Init()
    {
        Terrain t = Selection.activeObject as Terrain;
       
        terrains = Terrain.activeTerrains;
      
        if (terrains != null)
        {
            int size = terrains.Length;
            terrainPoss = new Vector3[size];
            for (int i = 0; i < size; i++)
            {
                terrainPoss[i] = terrains[i].transform.position;
            }
        }

        EditorWindow.GetWindow<ExportAllTerrain>().Show();
    }

    void OnGUI()
    {
        if (terrains==null)
        {
            GUILayout.Label("No terrain found");
            if (GUILayout.Button("Cancel"))
            {
                EditorWindow.GetWindow<ExportTerrain>().Close();
            }
            return;
        }
        saveFormat = (SaveFormat)EditorGUILayout.EnumPopup("Export Format", saveFormat);

        saveResolution = (SaveResolution)EditorGUILayout.EnumPopup("Resolution", saveResolution);

        if (GUILayout.Button("Export"))
        {
            Export();
        }
    }

    void Export()
    {
        int offset = 0;
        int poly_cnt=0;
        int vretex_cnt = 0;
        for (int n = 0; n < terrains.Length; n++)
        {
            TerrainData terrain = terrains[n].terrainData;
            int w = terrain.heightmapResolution;
            int h = terrain.heightmapResolution;
            int tRes = (int)Mathf.Pow(2, (int)saveResolution);
            w = (w - 1) / tRes + 1;
            h = (h - 1) / tRes + 1;
            if (saveFormat == SaveFormat.Triangles)
            {
                poly_cnt += (w - 1) * (h - 1) * 2;
            }
            else
            {
                poly_cnt *= (w - 1) * (h - 1) * 1;
            }
            vretex_cnt += w * h;
        }
        // Write vertices
        System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
        counter = tCount = 0;
        totalCount = (vretex_cnt * 2 + poly_cnt) / progressUpdateInterval;


        string fileName = EditorUtility.SaveFilePanel("Export .obj file", "", "Terrain", "obj");
        // Export to .obj
        StreamWriter sw = new StreamWriter(fileName);
        sw.WriteLine("# Unity terrain OBJ File");

        for (int n = 0; n < terrains.Length; n++) {
            TerrainData terrain = terrains[n].terrainData;
            Vector3 terrainPos = terrainPoss[n];
            int w = terrain.heightmapResolution;
            int h = terrain.heightmapResolution;
            Vector3 meshScale = terrain.size;
            int tRes = (int)Mathf.Pow(2, (int)saveResolution);
            meshScale = new Vector3(meshScale.x / (w - 1) * tRes, meshScale.y, meshScale.z / (h - 1) * tRes);
            Vector2 uvScale = new Vector2(1.0f / (w - 1), 1.0f / (h - 1));
            float[,] tData = terrain.GetHeights(0, 0, w, h);

            w = (w - 1) / tRes + 1;
            h = (h - 1) / tRes + 1;
            Vector3[] tVertices = new Vector3[w * h];
            Vector2[] tUV = new Vector2[w * h];

            int[] tPolys;

            if (saveFormat == SaveFormat.Triangles)
            {
                tPolys = new int[(w - 1) * (h - 1) * 6];
            }
            else
            {
                tPolys = new int[(w - 1) * (h - 1) * 4];
            }

            // Build vertices and UVs
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    tVertices[y * w + x] = Vector3.Scale(meshScale, new Vector3(-y, tData[x * tRes, y * tRes], x)) + terrainPos;
                    tUV[y * w + x] = Vector2.Scale(new Vector2(x * tRes, y * tRes), uvScale);
                }
            }

            int index = 0;
            if (saveFormat == SaveFormat.Triangles)
            {
                // Build triangle indices: 3 indices into vertex array for each triangle
                for (int y = 0; y < h - 1; y++)
                {
                    for (int x = 0; x < w - 1; x++)
                    {
                        // For each grid cell output two triangles
                        tPolys[index++] = (y * w) + x;
                        tPolys[index++] = ((y + 1) * w) + x;
                        tPolys[index++] = (y * w) + x + 1;

                        tPolys[index++] = ((y + 1) * w) + x;
                        tPolys[index++] = ((y + 1) * w) + x + 1;
                        tPolys[index++] = (y * w) + x + 1;
                    }
                }
            }
            else
            {
                // Build quad indices: 4 indices into vertex array for each quad
                for (int y = 0; y < h - 1; y++)
                {
                    for (int x = 0; x < w - 1; x++)
                    {
                        // For each grid cell output one quad
                        tPolys[index++] = (y * w) + x;
                        tPolys[index++] = ((y + 1) * w) + x;
                        tPolys[index++] = ((y + 1) * w) + x + 1;
                        tPolys[index++] = (y * w) + x + 1;
                    }
                }
            }
            try
            {


                for (int i = 0; i < tVertices.Length; i++)
                {
                    UpdateProgress();
                    StringBuilder sb = new StringBuilder("v ", 20);
                    // StringBuilder stuff is done this way because it's faster than using the "{0} {1} {2}"etc. format
                    // Which is important when you're exporting huge terrains.
                    sb.Append(tVertices[i].x.ToString()).Append(" ").
                       Append(tVertices[i].y.ToString()).Append(" ").
                       Append(tVertices[i].z.ToString());
                    sw.WriteLine(sb);
                }
                // Write UVs
                for (int i = 0; i < tUV.Length; i++)
                {
                    UpdateProgress();
                    StringBuilder sb = new StringBuilder("vt ", 22);
                    sb.Append(tUV[i].x.ToString()).Append(" ").
                       Append(tUV[i].y.ToString());
                    sw.WriteLine(sb);
                }
                {
                    StringBuilder sb = new StringBuilder("o ", 32);
                    sb.Append(terrains[n].name);
                    sw.WriteLine(sb);
                }
                if (saveFormat == SaveFormat.Triangles)
                {
                    // Write triangles
                    for (int i = 0; i < tPolys.Length; i += 3)
                    {
                        UpdateProgress();
                        StringBuilder sb = new StringBuilder("f ", 43);
                        sb.Append(tPolys[i] + 1 + offset).Append("/").Append(tPolys[i] + 1 + offset).Append(" ").
                           Append(tPolys[i + 1] + 1 + offset).Append("/").Append(tPolys[i + 1] + 1 + offset).Append(" ").
                           Append(tPolys[i + 2] + 1 + offset).Append("/").Append(tPolys[i + 2] + 1 + offset);
                        sw.WriteLine(sb);
                    }
                }
                else
                {
                    // Write quads
                    for (int i = 0; i < tPolys.Length; i += 4)
                    {
                        UpdateProgress();
                        StringBuilder sb = new StringBuilder("f ", 57);
                        sb.Append(tPolys[i] + 1 + offset).Append("/").Append(tPolys[i] + 1+ offset).Append(" ").
                           Append(tPolys[i + 1] + 1 + offset).Append("/").Append(tPolys[i + 1] + 1+ offset).Append(" ").
                           Append(tPolys[i + 2] + 1 + offset).Append("/").Append(tPolys[i + 2] + 1+ offset).Append(" ").
                           Append(tPolys[i + 3] + 1+ offset).Append("/").Append(tPolys[i + 3] + 1+ offset);
                        sw.WriteLine(sb);
                    }
                }
            }
            catch (Exception err)
            {
                Debug.Log("Error saving file: " + err.Message);
            }
            offset += w * h;
        }
        sw.Close();

        terrains = null;
        EditorUtility.DisplayProgressBar("Saving file to disc.", "This might take a while...", 1f);
        EditorWindow.GetWindow<ExportAllTerrain>().Close();
        EditorUtility.ClearProgressBar();
    }

    void UpdateProgress()
    {
        if (counter++ == progressUpdateInterval)
        {
            counter = 0;
            EditorUtility.DisplayProgressBar("Saving...", "", Mathf.InverseLerp(0, totalCount, ++tCount));
        }
    }
}
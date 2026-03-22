using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class ProceduralGenerator : MonoBehaviour
{
    [Header("Starting Tile")]
    public GameObject startingTile;

    [Header("Tile Prefabs")]
    public GameObject[] normalTiles;
    public GameObject[] dangerSpikesTiles;
    public GameObject[] dangerEnemyTiles;
    public GameObject[] damagingTiles;
    public GameObject[] lightningPickupTiles;
    public GameObject[] keyTiles;

    [Header("Level")]
    public LevelDefinition levelDefinition;
    public GameObject startPosition;

    [Header("Generation Settings")]
    public int seed = 0;

    private readonly List<GameObject> spawnedTiles = new();

    private void Start() => GenerateLevel();

    public void GenerateLevel()
    {
        ClearLevel();

        if (startPosition == null) { Debug.LogError("ERROR: No start position assigned."); return; }
        if (levelDefinition == null) { Debug.LogError("ERROR: No level definition assigned."); return; }
        if (startingTile == null) { Debug.LogError("ERROR: starting tile assigned."); return; }

        if (seed != 0) Random.InitState(seed);

        // starting tile
        GameObject first = Instantiate(startingTile, startPosition.transform.position, Quaternion.identity);
        first.name = "Tile_Start";
        spawnedTiles.Add(first);

        Transform currentExit = GetRandomPointWithTag(first, "ExitPoint");
        if (currentExit == null) { Debug.LogError("ERROR: Starting tile has no ExitPoint."); return; }

        // chain tiles from the level definition
        List<TileType> sequence = levelDefinition.BuildSequence();
        if (sequence.Count == 0) { Debug.LogError("ERROR: Level definition has no segments."); return; }

        for (int i = 0; i < sequence.Count; i++)
        {
            GameObject nextTile = SpawnTile(sequence[i], Vector3.zero, $"Tile_{i + 1}");
            if (nextTile == null) continue;

            Transform nextEntry = GetRandomPointWithTag(nextTile, "EntryPoint");
            if (nextEntry == null)
            {
                Debug.LogWarning($"ERROR: '{nextTile.name}' has no EntryPoint");
                Destroy(nextTile);
                continue;
            }

            AlignTile2D(nextTile.transform, nextEntry, currentExit);
            spawnedTiles.Add(nextTile);

            Transform nextExit = GetRandomPointWithTag(nextTile, "ExitPoint");
            if (nextExit == null)
            {
                Debug.LogWarning($"ERROR: '{nextTile.name}' has no ExitPoint");
                break;
            }

            currentExit = nextExit;
        }
    }

    public void ClearLevel()
    {
        foreach (var tile in spawnedTiles)
        {
            if (tile != null)
            {
                Destroy(tile);
            }
            spawnedTiles.Clear();
        }
    }

    private GameObject SpawnTile(TileType type, Vector3 position, string tileName)
    {
        GameObject[] pool = type switch
        {
            TileType.Normal => normalTiles,
            TileType.DangerSpikes => dangerSpikesTiles,
            TileType.DangerEnemy => dangerEnemyTiles,
            TileType.Damaging => damagingTiles,
            TileType.LightningPickup => lightningPickupTiles,
            TileType.Key => keyTiles,
            _ => null
        };

        if (pool == null || pool.Length == 0)
        {
            Debug.LogWarning($"ERROR:  No prefabs assigned for tile type '{type}'.");
            return null;
        }

        GameObject tile = Instantiate(pool[Random.Range(0, pool.Length)], position, Quaternion.identity);
        tile.name = $"{tileName}_{type}";
        return tile;
    }

    private static void AlignTile2D(Transform tileRoot, Transform entryPoint, Transform exitPoint)
    {
        Vector2 delta = (Vector2)exitPoint.position - (Vector2)entryPoint.position;
        tileRoot.position += (Vector3)delta;
    }

    private static Transform GetRandomPointWithTag(GameObject tile, string tag)
    {
        Transform[] candidates = tile.GetComponentsInChildren<Transform>(includeInactive: true)
                                     .Where(t => t != tile.transform && t.CompareTag(tag))
                                     .ToArray();
        return candidates.Length == 0 ? null : candidates[Random.Range(0, candidates.Length)];
    }
}
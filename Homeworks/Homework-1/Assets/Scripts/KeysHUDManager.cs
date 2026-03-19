using UnityEngine;
using UnityEngine.UI;

public class KeysHUDManager : MonoBehaviour
{
    public static KeysHUDManager Instance;

    public GameObject keyPrefab;
    public Image[] keyImages;
    public Sprite emptyKeySprite;
    public Sprite filledKeySprite;

    private int keysCollected = 0;

    public void Init(int nrOfKeys)
    {
        // Clear any existing children
        foreach (Transform child in transform)
        {
            Destroy(child.gameObject);
        }

        keysCollected = 0;
        keyImages = new Image[nrOfKeys];

        for (int i = 0; i < nrOfKeys; i++)
        {
            GameObject keyObj = Instantiate(keyPrefab, transform);
            Image img = keyObj.GetComponent<Image>();
            img.sprite = emptyKeySprite;
            keyImages[i] = img;
        }
    }

    void Awake()
    {
        Instance = this;
    }

    public void CollectKey()
    {
        if (keysCollected >= keyImages.Length) return;

        keyImages[keysCollected].sprite = filledKeySprite;
        keysCollected++;
    }
}
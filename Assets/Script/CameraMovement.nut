class CameraMovement extends CSEngineScript {

    playerTransform = null;
    myTransform = null;

    function Init() {
        local camera = gameObject.GetComponent<CameraComponent>();
        local playerObject = gameObject.Find("player object");
        camera.SetTarget(playerObject);

        playerTransform = playerObject.GetTransform();
        myTransform = gameObject.GetTransform();
    }

    function Tick(elapsedTime) {
        myTransform.position.Set(playerTransform.position.x, 40, playerTransform.position.z + 1.5708);
    }
}
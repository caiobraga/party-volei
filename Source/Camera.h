#include <SDL.h>
#include "Math.h"

class Camera {
private:
    Vector2 mCameraPos;
    float mZoom;

    // SDL specific members
    SDL_Renderer* mRenderer;

public:
    Camera(SDL_Renderer* renderer) : mCameraPos(Vector2(0, 0)), mZoom(1.0f), mRenderer(renderer) {}

    void SetPosition(const Vector2& position) {
        mCameraPos = position;
    }

    Vector2 GetPosition() const {
        return mCameraPos;
    }

    void SetZoom(float zoomLevel) {
        mZoom = zoomLevel;
    }

    float GetZoom() const {
        return mZoom;
    }

    // Function for smooth zoom effect
    void ZoomTo(const Vector2& targetPosition, float timeInSeconds, float deltaTime) {
        // Calculate the change in zoom per second to achieve the target zoom level
        Vector2 currentPosition = mCameraPos;
        float currentZoom = mZoom;

        Vector2 distance = targetPosition - currentPosition;
        float targetZoom = CalculateTargetZoom(distance);

        float zoomPerSecond = (targetZoom - currentZoom) / timeInSeconds;

        float elapsedTime = 0.0f;
        while (elapsedTime < timeInSeconds) {
            currentZoom += zoomPerSecond * deltaTime;

            // Update the zoom
            SetZoom(currentZoom);

            // Clear the screen
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);

            // Render the scene with the updated zoom and position using SDL render operations
            // RenderScene();

            // Update the display
            SDL_RenderPresent(mRenderer);

            elapsedTime += deltaTime;
        }

        // Ensure final zoom level is set to the target
        SetZoom(targetZoom);
    }

private:
    // Helper function to calculate the target zoom level based on distance
    float CalculateTargetZoom(const Vector2& distance) const {
        // Calculate the zoom based on distance or any other criterion you desire
        float targetZoom = 1.0f + distance.Length() * 0.1f; // Adjust the scale factor as needed
        return targetZoom;
    }

    // Implement your rendering function to display the scene with updated camera parameters using SDL render operations
    // void RenderScene() {
    //     // Render your scene using SDL render operations, considering camera position and zoom level
    // }
};

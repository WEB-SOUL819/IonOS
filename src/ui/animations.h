#ifndef IONOS_ANIMATIONS_H
#define IONOS_ANIMATIONS_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - ANIMATIONS
// Smooth transition and animation effects
// ============================================================================

enum AnimationType {
    ANIM_NONE = 0,
    ANIM_FADE_IN = 1,
    ANIM_FADE_OUT = 2,
    ANIM_SLIDE_LEFT = 3,
    ANIM_SLIDE_RIGHT = 4,
    ANIM_SLIDE_UP = 5,
    ANIM_SLIDE_DOWN = 6,
    ANIM_ZOOM_IN = 7,
    ANIM_ZOOM_OUT = 8,
    ANIM_BOUNCE = 9,
    ANIM_PULSE = 10
};

enum EasingType {
    EASE_LINEAR = 0,
    EASE_IN_QUAD = 1,
    EASE_OUT_QUAD = 2,
    EASE_INOUT_QUAD = 3,
    EASE_IN_CUBIC = 4,
    EASE_OUT_CUBIC = 5,
    EASE_INOUT_CUBIC = 6
};

struct Animation {
    AnimationType type;
    EasingType easing;
    uint32_t duration_ms;
    uint32_t start_time;
    bool active;
    void (*callback)(float progress);  // Progress callback (0.0 to 1.0)
};

class AnimationManager {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Animation control
    static Animation* createAnimation(AnimationType type, uint32_t duration_ms);
    static void startAnimation(Animation *anim);
    static void stopAnimation(Animation *anim);
    static void updateAnimations();

    // Easing functions
    static float easeLinear(float t);
    static float easeInQuad(float t);
    static float easeOutQuad(float t);
    static float easeInOutQuad(float t);
    static float easeInCubic(float t);
    static float easeOutCubic(float t);
    static float easeInOutCubic(float t);

    // Utility
    static float getProgress(const Animation *anim);
    static bool isAnimationComplete(const Animation *anim);

    // Debug
    static void printDebugInfo();

private:
    static const uint8_t MAX_ANIMATIONS = 8;
    static Animation animations[MAX_ANIMATIONS];
    static uint8_t active_count;

    static float applyEasing(float t, EasingType easing);
};

#endif // IONOS_ANIMATIONS_H

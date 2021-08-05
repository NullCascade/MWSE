mge
====================================================================================================

The mge library allows MGE XE mwscript functions to be called. This is not always ideal, and this library is deprecated.

Functions
----------------------------------------------------------------------------------------------------

`clearHUD <mge/clearHUD.html>`_ (`function`_)
    Clears elements from the HUD. Wrapper for MGEWipeHUDElements.

`disableCameraShake <mge/disableCameraShake.html>`_ (`function`_)
    Disables MGE XE's camera shake effect. Wrapper for mwscript's MGEDisableCameraShake.

`disableHUD <mge/disableHUD.html>`_ (`function`_)
    Disables a HUD element. If a HUD ID is provided, this is a warpper for MGEDisableHUD, otherwise it is a wrapper for MGENIDDisableHUD.

`disableShader <mge/disableShader.html>`_ (`function`_)
    Disables a shader. Wrapper for mwscript's MGEDisableShader.

`disableZoom <mge/disableZoom.html>`_ (`function`_)
    Disables the MGE XE zoom effect. Wrapper for MGEDisableZoom.

`enableCameraShake <mge/enableCameraShake.html>`_ (`function`_)
    Enables MGE XE's camera shake effect. Wrapper for mwscript's MGEEnableCameraShake.

`enableHUD <mge/enableHUD.html>`_ (`function`_)
    Enables a HUD element. If a HUD ID is provided, this is a warpper for MGEEnableHUD, otherwise it is a wrapper for MGENIDEnableHUD.

`enableShader <mge/enableShader.html>`_ (`function`_)
    Enables a shader. Wrapper for mwscript's MGEEnableShader.

`enableZoom <mge/enableZoom.html>`_ (`function`_)
    Enables the MGE XE zoom effect. Wrapper for MGEEnableZoom.

`freeHUD <mge/freeHUD.html>`_ (`function`_)
    Frees a HUD element. If a HUD ID is provided, this is a warpper for MGEFreeHUD, otherwise it is a wrapper for MGENIDFreeHUD.

`fullscreenHUD <mge/fullscreenHUD.html>`_ (`function`_)
    Makes a HUD element fullscreen. If a HUD ID is provided, this is a warpper for MGEFullscreenHUD, otherwise it is a wrapper for MGENIDFullscreenHUD.

`getScreenHeight <mge/getScreenHeight.html>`_ (`function`_)
    Gets the window's vertical resolution. Wrapper for mwscript's MGEGetHeight.

`getScreenRotation <mge/getScreenRotation.html>`_ (`function`_)
    Wrapper for MGEGetScreenRotation.

`getScreenWidth <mge/getScreenWidth.html>`_ (`function`_)
    Gets the window's horizontal resolution. Wrapper for mwscript's MGEGetWidth.

`getUIScale <mge/getUIScale.html>`_ (`function`_)
    Returns the UI scaling used.

`getVersion <mge/getVersion.html>`_ (`function`_)
    Gets the MGE version. Wrapper for MGEGetVersion.

`getZoom <mge/getZoom.html>`_ (`function`_)
    Wrapper for MGEGetZoom.

`loadHUD <mge/loadHUD.html>`_ (`function`_)
    Loads a HUD element. This is a warpper for MGELoadHUD.

`log <mge/log.html>`_ (`function`_)
    This function writes information to the mgeXE.log file in the user's installation directory. Wrapper for mwscript's MGELog function.

`modScreenRotation <mge/modScreenRotation.html>`_ (`function`_)
    Wrapper for MGERotateScreenBy.

`positionHUD <mge/positionHUD.html>`_ (`function`_)
    Positions a HUD element. If a HUD ID is provided, this is a warpper for MGEPositionHUD, otherwise it is a wrapper for MGENIDPositionHUD.

`scaleHUD <mge/scaleHUD.html>`_ (`function`_)
    Scales a HUD element. If a HUD ID is provided, this is a warpper for MGEScaleHUD, otherwise it is a wrapper for MGENIDScaleHUD.

`selectHUD <mge/selectHUD.html>`_ (`function`_)
    Selects a HUD element, to assume for future HUD calls.

`setCameraShakeAcceleration <mge/setCameraShakeAcceleration.html>`_ (`function`_)
    Wrapper for MGECameraShakeZoom.

`setCameraShakeMagnitude <mge/setCameraShakeMagnitude.html>`_ (`function`_)
    Wrapper for MGESetCameraShakeMagnitude.

`setHUDEffect <mge/setHUDEffect.html>`_ (`function`_)
    Sets the effect for a HUD element. If a HUD ID is provided, this is a warpper for MGEChangeHUDEffect, otherwise it is a wrapper for MGENIDChangeHUDEffect.

`setHUDEffectFloat <mge/setHUDEffectFloat.html>`_ (`function`_)
    Sets the effect float variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectFloat.

`setHUDEffectLong <mge/setHUDEffectLong.html>`_ (`function`_)
    Sets the effect long variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectLong.

`setHUDEffectVector4 <mge/setHUDEffectVector4.html>`_ (`function`_)
    Sets the effect vector variable for a HUD element. This is a wrapper for MGENIDSetHUDEffectVec.

`setHUDTexture <mge/setHUDTexture.html>`_ (`function`_)
    Sets the texture for a given HUD element. If a HUD ID is provided, this is a warpper for MGEChangeHUDTexture, otherwise it is a wrapper for MGENIDChangeHUDTexture.

`setScreenRotation <mge/setScreenRotation.html>`_ (`function`_)
    Wrapper for MGERotateScreen.

`setShaderFloat <mge/setShaderFloat.html>`_ (`function`_)
    Sets a shader's float variable. Wrapper for mwscript's MGEShaderSetFloat.

`setShaderLong <mge/setShaderLong.html>`_ (`function`_)
    Sets a shader's long variable. Wrapper for mwscript's MGEShaderSetLong.

`setShaderVector4 <mge/setShaderVector4.html>`_ (`function`_)
    Sets a shader's vector variable. Wrapper for mwscript's MGEShaderSetVector.

`setZoom <mge/setZoom.html>`_ (`function`_)
    Wrapper for MGEZoom, or MGESetZoom if set to animate.

`startScreenRotation <mge/startScreenRotation.html>`_ (`function`_)
    Enables MGE XE's screen spin effect. Wrapper for mwscript's MGEScreenSpin.

`stopScreenRotation <mge/stopScreenRotation.html>`_ (`function`_)
    Disables MGE XE's screen spin effect. Wrapper for mwscript's MGEStopSpinSpin.

`stopZoom <mge/stopZoom.html>`_ (`function`_)
    Wrapper for MGEStopZoom.

`toggleZoom <mge/toggleZoom.html>`_ (`function`_)
    Toggles the MGE XE zoom effect. Wrapper for MGEToggleZoom.

`unselectHUD <mge/unselectHUD.html>`_ (`function`_)
    Cancels selection for a HUD element. Wrapper for mwscript's MGECancelWithHUD.

`zoomIn <mge/zoomIn.html>`_ (`function`_)
    Wrapper for MGEZoomIn, or MGEZoomInBy if an amount is provided.

`zoomOut <mge/zoomOut.html>`_ (`function`_)
    Wrapper for MGEZoomOut, or MGEZoomOutBy if an amount is provided.



.. toctree::
    :hidden:
    :maxdepth: 1

    mge/clearHUD
    mge/disableCameraShake
    mge/disableHUD
    mge/disableShader
    mge/disableZoom
    mge/enableCameraShake
    mge/enableHUD
    mge/enableShader
    mge/enableZoom
    mge/freeHUD
    mge/fullscreenHUD
    mge/getScreenHeight
    mge/getScreenRotation
    mge/getScreenWidth
    mge/getUIScale
    mge/getVersion
    mge/getZoom
    mge/loadHUD
    mge/log
    mge/modScreenRotation
    mge/positionHUD
    mge/scaleHUD
    mge/selectHUD
    mge/setCameraShakeAcceleration
    mge/setCameraShakeMagnitude
    mge/setHUDEffect
    mge/setHUDEffectFloat
    mge/setHUDEffectLong
    mge/setHUDEffectVector4
    mge/setHUDTexture
    mge/setScreenRotation
    mge/setShaderFloat
    mge/setShaderLong
    mge/setShaderVector4
    mge/setZoom
    mge/startScreenRotation
    mge/stopScreenRotation
    mge/stopZoom
    mge/toggleZoom
    mge/unselectHUD
    mge/zoomIn
    mge/zoomOut

.. _`function`: ../../lua/type/function.html

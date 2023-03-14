#pragma once

#include "Common.h"
#include "OVR_ErrorCode.h"

#include <openxr/openxr.h>
#include <map>
#include <vector>

class Runtime
{
public:
	static Runtime& Get();

	enum Hack
	{
		// Hack: SteamVR runtime doesn't support the Oculus Touch interaction profile.
		// Use the Valve Index interaction profile instead.
		HACK_VALVE_INDEX_PROFILE,
		// Hack: WMR runtime doesn't support the Oculus Touch interaction profile.
		// Use the WMR motion controller interaction profile instead.
		HACK_WMR_PROFILE,
		// Hack: Some games only call GetRenderDesc once before the session is fully initialized.
		// Therefore we need to force the fallback field-of-view query so we get full ViewPoses.
		HACK_FORCE_FOV_FALLBACK,
		// Hack: SteamVR runtime allocates a buffer that is too big for the visibility line loop.
		// Causes the rest of the buffer to be filled with uninitialized coordinates.
		HACK_BROKEN_LINE_LOOP,
		// Hack: Oculus runtime visibility masks are in Normalized Device Coordinates.
		// Simply set the projection matrix to the identity matrix as a workaround.
		HACK_NDC_MASKS,
		// Hack: SteamVR runtime ignores haptic pulses with a long duration.
		// Set the duration to the minimum duration as a workaround.
		HACK_MIN_HAPTIC_DURATION,
		// Hack: WMR runtime doesn't allow views to be located without the session running.
		// Wait for the session to become ready instead.
		HACK_WAIT_FOR_SESSION_READY,
	};

	bool UseHack(Hack hack);
	ovrResult CreateInstance(XrInstance* out_Instance, const ovrInitParams* params);
	bool Supports(const char* extensionName);

	bool VisibilityMask;
	bool CompositionDepth;
	bool CompositionCube;
	bool CompositionCylinder;
	bool AudioDevice;
	bool ColorSpace;

	uint32_t MinorVersion;

private:
	struct HackInfo
	{
		const char* m_filename;		// The filename of the main executable
		const char* m_runtime;		// The name of the runtime
		Hack m_hack;				// Which hack is it?
		XrVersion m_versionstart;	// When it started
		XrVersion m_versionend;		// When it ended
		bool m_usehack;				// Should it use the hack?
	};

	static const char* s_required_extensions[];
	static const char* s_optional_extensions[];
	static HackInfo s_known_hacks[];

	std::map<Hack, HackInfo> m_hacks;
	std::vector<const char*> m_extensions;
};

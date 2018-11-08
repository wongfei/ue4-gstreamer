#include "GstCoreImpl.h"

extern "C" {
	#include <gst/gst.h>
	#include <gst/gstregistry.h>
}

bool FGstCoreImpl::Init(const char* BinPath, const char* PluginPath)
{
	int argc = 0;
	char** argv = nullptr;
	gst_init(&argc, &argv);

	guint major, minor, micro, nano;
	gst_version(&major, &minor, &micro, &nano);
	GST_LOG_DBG_A("GStreamer: Version %d.%d.%d", major, minor, micro);

	GstRegistry* registry = gst_registry_get();
	gst_registry_scan_path(registry, BinPath);
	gst_registry_scan_path(registry, PluginPath);

	return gst_is_initialized();
}

void FGstCoreImpl::Deinit()
{
	gst_deinit();
}

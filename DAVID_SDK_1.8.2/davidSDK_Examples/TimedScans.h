/// @file TimedScans.h
///
/// Takes a scan every x minute.

namespace examples {

/// Take a scan every 'waitTimeInSeconds' seconds.
/// This example assumes that the scanner is calibrated and camera and projector are connected and correctly configured.
/// The scans are saved in the current directory.
///
/// @param[in] waitTimeInSeconds Wait time in seconds until next scan.
/// @param[in] scanCount Total number of scans to be taken.
///
void main_TimedScans(double waitTimeInSeconds, int scanCount);

} // namespace
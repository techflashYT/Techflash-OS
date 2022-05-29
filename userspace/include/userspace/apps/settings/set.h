#ifndef USERSPACE_APPS_SETTINGS_SET_HPP
#define USERSPACE_APPS_SETTINGS_SET_HPP
struct __SettingsAPI_t {
	static struct __Terminal_t {
		constexpr uint16_t getBufferSize() { // TODO: Find some way to make this whole trainwreck from here up to putchar not be const
			return 2000;
		}
	} terminal;
} settingsAPI;
struct Settings_t {
	static struct Terminal_t {
		static const uint16_t bufferSize = settingsAPI.terminal.getBufferSize();
	} terminal;
} settings;
#endif

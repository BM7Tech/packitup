// launcher.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI
WinMain (HINSTANCE, HINSTANCE, LPSTR, int)
{
  WCHAR path[MAX_PATH];
  // 1) Get full path to this .exe
  if (!GetModuleFileNameW (NULL, path, MAX_PATH))
    return 1;

  // 2) Strip off the filename, leaving the dir (e.g. "C:\Program
  // Files\PackItUP\")
  for (int i = lstrlenW (path) - 1; i >= 0; --i)
    {
      if (path[i] == L'\\')
        {
          path[i + 1] = L'\0';
          break;
        }
    }

  SetEnvironmentVariableW (L"TEXTDOMAINDIR", path);
  // 3) Build schema-dir path and set the env var
  WCHAR schema_dir[MAX_PATH];
  lstrcpyW (schema_dir, path);
  lstrcatW (schema_dir, L"share\\glib-2.0\\schemas");
  SetEnvironmentVariableW (L"GSETTINGS_SCHEMA_DIR", schema_dir);

  // 4) Build the real exe’s path: "<same dir>packitup-real.exe"
  WCHAR real_exe[MAX_PATH];
  lstrcpyW (real_exe, path);
  lstrcatW (real_exe, L"packitup-real.exe");

  // 5) Launch it (no console, GUI subsystem)
  STARTUPINFOW si = { sizeof (si) };
  PROCESS_INFORMATION pi;
  if (!CreateProcessW (real_exe, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si,
                       &pi))
    {
      MessageBoxW (NULL, L"Failed to launch PackItUP", L"Error",
                   MB_OK | MB_ICONERROR);
      return 1;
    }
  // Wait for it to finish, then return its exit code
  WaitForSingleObject (pi.hProcess, INFINITE);
  DWORD code = 0;
  GetExitCodeProcess (pi.hProcess, &code);
  CloseHandle (pi.hProcess);
  CloseHandle (pi.hThread);
  return (int)code;
}

# ifndef PROOF_EDITOR_HEADER
# define PROOF_EDITOR_HEADER

# include "BuildSets.h"
/*	PROTECTION_DISABLED  */

# include "Resource.h"
# include "ExitCodes.h"
# include "..\\..\\CommonIDs.h"
# include "..\\Lang\\CommonLangIDs.h"

# include <Windows.h>
# include <Math.h>

# include "..\\Utilities\\Files\\FilesClass.h"
# include "..\\Utilities\\Locale\\LocaleClass.h"
# include "..\\Utilities\\NotifyMessageBox\\NotifyMessageBoxClass.h"
# include "..\\Utilities\\Settings\\SettingsClass.h"
# include "..\\Utilities\\SplashWindow\\SplashWindowClass.h"
//# include "..\\\\ImageClass.h"
# include "..\\..\\..\\License\\Application\\Check.h"

# include "MainWindowClass.h"
# include "EditWindowClass.h"

# define STRINGIFY(X)	#X

# define MASK_ALPHA_VALUE			220

LRESULT CALLBACK MainProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

# endif // PROOF_EDITOR_HEADER

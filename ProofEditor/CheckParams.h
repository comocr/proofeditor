# define N_FILES					3

// Paths
# define THIS_PATH					L"ProofEditor.exe"
# define SPLASH_PATH				L"Splash.bmp"
# define REGISTRATION_PATH			L"Registration.dll"

// Sizes
# define THIS_SIZE					271360
# define SPLASH_SIZE				583256
# define REGISTRATION_SIZE			163840

// CheckSums
# define THIS_CRC_KEY				0xC2C2C2C2
# define SPLASH_CRC					0xCB9A686E
# ifdef _DEBUG	
	# ifdef _M_X64					
		# define REGISTRATION_CRC	0xE14FE3DF
	# else
		# define REGISTRATION_CRC	0x0
	# endif
# else
	# ifdef _M_X64					
		# define REGISTRATION_CRC	0xE46E0B70
	# else
		# define REGISTRATION_CRC	0xB1A063D3
	# endif
# endif

# define CKSM_LIST					{REGISTRATION_CRC, SPLASH_CRC, THIS_CRC_KEY}
# define PATH_LIST					{REGISTRATION_PATH, SPLASH_PATH, THIS_PATH}
# define SIZE_LIST					{REGISTRATION_SIZE, SPLASH_SIZE, THIS_SIZE}

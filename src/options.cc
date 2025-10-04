// SPDX-License-Identifier: GPL-3.0-or-later

namespace Options
{

SDL_Keycode	up;
SDL_Keycode	right;
SDL_Keycode	down;
SDL_Keycode	left;
SDL_Keycode	zoomIn;
SDL_Keycode	zoomOut;
SDL_Keycode	pencil;
SDL_Keycode	bucket;
SDL_Keycode	eyeDropper;
SDL_Keycode	select;
SDL_Keycode	boxSelect;

void
Default()
{
	up				= SDLK_w;
	right			= SDLK_d;
	down			= SDLK_s;
	left			= SDLK_a;
	zoomIn		= SDLK_1;
	zoomOut		= SDLK_2;
	pencil		= SDLK_e;
	bucket		= SDLK_b;
	eyeDropper	= SDLK_q;
	select		= SDLK_c;
	boxSelect	= SDLK_g;
}

i32
Read()
{
	std::filesystem::path	path	= Path();
	FILE*							file	= fopen(path.c_str(), "rb");
	if (!file)
	{
		// do not display error, let the program instead try to create a fresh
		// configuration file with defaults.
		return (1);
	}
	
	if (ZTGL::OptionKeycode(up, file, "up")
		|| ZTGL::OptionKeycode(right, file, "right")
		|| ZTGL::OptionKeycode(down, file, "down")
		|| ZTGL::OptionKeycode(left, file, "left")
		|| ZTGL::OptionKeycode(zoomIn, file, "zoomIn")
		|| ZTGL::OptionKeycode(zoomOut, file, "zoomOut")
		|| ZTGL::OptionKeycode(pencil, file, "pencil")
		|| ZTGL::OptionKeycode(bucket, file, "bucket")
		|| ZTGL::OptionKeycode(eyeDropper, file, "eyeDropper")
		|| ZTGL::OptionKeycode(select, file, "select")
		|| ZTGL::OptionKeycode(boxSelect, file, "boxSelect"))
	{
		ZTGL::Error("options: failed to read options, resetting to default!");
		fclose(file);
		return (1);
	}
	
	fclose(file);
	return (0);
}

i32
Write()
{
	std::filesystem::path	path	= Path();
	FILE*							file	= fopen(path.c_str(), "wb");
	if (!file)
	{
		ZTGL::Error("options: failed to open options file to write!");
		return (1);
	}
	
	std::string	upName			= SDL_GetKeyName(up);
	std::string	rightName		= SDL_GetKeyName(right);
	std::string	downName			= SDL_GetKeyName(down);
	std::string	leftName			= SDL_GetKeyName(left);
	std::string	zoomInName		= SDL_GetKeyName(zoomIn);
	std::string	zoomOutName		= SDL_GetKeyName(zoomOut);
	std::string	pencilName		= SDL_GetKeyName(pencil);
	std::string	bucketName		= SDL_GetKeyName(bucket);
	std::string	eyeDropperName	= SDL_GetKeyName(eyeDropper);
	std::string	selectName		= SDL_GetKeyName(select);
	std::string	boxSelectName	= SDL_GetKeyName(boxSelect);
	
	fprintf(
		file,
		"up = %s\n"
		"right = %s\n"
		"down = %s\n"
		"left = %s\n"
		"zoomIn = %s\n"
		"zoomOut = %s\n"
		"pencil = %s\n"
		"bucket = %s\n"
		"eyeDropper = %s\n"
		"select = %s\n"
		"boxSelect = %s\n",
		upName.c_str(),
		rightName.c_str(),
		downName.c_str(),
		leftName.c_str(),
		zoomInName.c_str(),
		zoomOutName.c_str(),
		pencilName.c_str(),
		bucketName.c_str(),
		eyeDropperName.c_str(),
		selectName.c_str(),
		boxSelectName.c_str()
	);
	
	fclose(file);
	return (0);
}

std::filesystem::path
Path()
{
	// TODO: use system paths on Windows / Linux.
	return ("2de.conf");
}

}

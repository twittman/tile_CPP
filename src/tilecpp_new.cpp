#include <Magick++.h>
#include <cxxopts.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <map>
#include <filesystem>
#include "util.hpp"

#ifdef __APPLE__
    #define __int64 __int64_t
    #define __int8 __int8_t
#endif

void cropTiles( int tileH, int tileW,
				std::string& outFileDir,
				std::string& inFileNoEXT,
				Magick::Blob& croppedNameBlob,
				int widthTile, int heightTile )

{
#pragma omp parallel for 

	for ( auto i = 0; i < tileH; ++i )
	{
		for ( auto j = 0; j < tileW; ++j )
		{
			std::string outputName = outFileDir + inFileNoEXT + "_" + std::to_string( __int8( i ) * tileW ) + "_" + std::to_string( j ) + ".png";

			try {
				Magick::Image cCrop( croppedNameBlob );
				cCrop.crop( Magick::Geometry( widthTile, heightTile,
											  widthTile * __int8( j ), heightTile * __int8( i ) ) );
				cCrop.write( outputName );
			}
			catch ( Magick::Exception& error )
			{
				throw Magick::Exception( error );
			}
		}
	}
}

void cropToDivisor( std::string& inFileEXT,
					std::string& inFileNoEXT,
					std::string& outFileDir,
					std::string& ExtensionType,
					int& imDim, int& buggary )
{
	try
	{
		if ( twitls::imgExt::is_image_extension( ExtensionType ) ) {

			Magick::Image image, croppedImage;
			Magick::Blob croppedNameBlob;
			image.read( inFileEXT );

			size_t sizeWidth = image.baseColumns();
			size_t sizeHeight = image.baseRows();
			ssize_t sizeDiffW;
			ssize_t sizeDiffH;
			size_t sizePadW;
			size_t sizePadH;
			double divW;
			double divH;
			size_t croppedWidth;
			size_t croppedHeight;
			size_t tileW;
			size_t tileH;
			size_t widthTile;
			size_t heightTile;

			auto t1 = std::chrono::high_resolution_clock::now();
			try {

				sizeDiffW = static_cast<ssize_t>( twitls::count::diff( sizeWidth, imDim ));
				sizeDiffH = static_cast<ssize_t>( twitls::count::diff( sizeHeight, imDim ));
				sizePadW = static_cast<size_t>(twitls::count::padd( sizeDiffW, imDim ));
				sizePadH = static_cast<size_t>( twitls::count::padd( sizeDiffH, imDim ));

				divW = twitls::count::divv( sizeWidth, sizePadW );
				divH = twitls::count::divv( sizeHeight, sizePadH );

				if ( !sizeWidth % imDim == 0 || !sizeHeight % imDim == 0 ) {
					image.magick( "PNG" );
					image.crop( Magick::Geometry( sizePadW, sizePadH, divW / 2, divH / 2 ) );
					image.repage();
					image.write( &croppedNameBlob );
					croppedImage.read( croppedNameBlob );
				}
				else if ( sizeWidth % imDim == 0 || sizeHeight % imDim == 0 ) {
					croppedImage.read( inFileEXT );
					croppedImage.repage();
				}

				croppedWidth = croppedImage.columns();
				croppedHeight = croppedImage.rows();

				// check for division by zero and evade it!
				/////////
				if ( croppedWidth / imDim == 0 ) {
					tileW = 1;
				}
				else {
					tileW = croppedWidth / imDim;
				}
				
				if ( croppedHeight / imDim == 0 ) {
					tileH = 1;
				}
				else {
					tileH = croppedHeight / imDim;
				}


				widthTile = (int)( croppedImage.columns() / tileW );
				heightTile = (int)( croppedImage.rows() / tileH );

				cropTiles( tileH, tileW, outFileDir, inFileNoEXT, croppedNameBlob, widthTile, heightTile );
				std::cout << inFileEXT << " has been cropped." << "\n";

			}
			catch ( Magick::Exception& error )
			{
				throw Magick::Exception( error );
			}

			auto t2 = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
			if ( buggary == 1 ) {
				std::cout << "\n" << inFileEXT << " Processed with: " << "\n" << "Crop w: " << sizePadW << "\n" << "Crop h: " << sizePadH << std::endl;
				std::cout << "Horizontal tiles: " << tileW << std::endl;
				std::cout << "Vertical tiles: " << tileH << std::endl;
				std::cout << "Tile width: " << widthTile << "\n";
				std::cout << "Tile height: " << heightTile << "\n";
				std::cout << "Pad width: " << sizePadW << "\n";
				std::cout << "Pad height: " << sizePadH << "\n";

				std::cout << "Total tiles count: " << tileH * tileW << std::endl;
				std::cout << "Time taken for this image: " << duration / 1000.0 << " seconds" << "\n" << std::endl;
			}
			else {
				//std::cout << "\n" << inFileEXT << " Processed";
			}
		}
	}
	catch ( Magick::Exception& error )
	{
		throw Magick::Exception( error );
	}
}

void runOnDir( std::string& input,
			   std::filesystem::path& inputDir,
			   std::string& output,
			   std::filesystem::path& outputDir,
			   int& imDim, int& buggary )
{
	try {
		for ( const auto& entry : std::filesystem::directory_iterator( input ) ) {
			if ( is_regular_file( entry.path() ) ) {

				std::string ExtensionType = entry.path().extension().string();

				std::string inFileEXT = entry.path().string();
				const __int64 index1 = inFileEXT.find_last_of( "/\\" );
				std::string fileNoPathEXT = inFileEXT.substr( index1 + 1 );
				std::string inFileNoEXT = entry.path().stem().string();
				std::string croppedName = inFileNoEXT + "_cropped.png";
				std::string outFileDir = outputDir.string();
				std::string outFile = outFileDir + '\\' + fileNoPathEXT;

				cropToDivisor( inFileEXT, inFileNoEXT, outFileDir, ExtensionType, imDim, buggary );

			}
		}
	}
	catch ( const std::exception& e ) {
		std::cout << e.what();
	}
}


int main( int argc, char** argv )
{
	cxxopts::Options options( "Test" );
	options.add_options()
		( "i, input", "input directory", cxxopts::value<std::string>() )
		( "o, output", "output directory", cxxopts::value<std::string>() )
		( "c, crop", "tile size", cxxopts::value<int>() )
		( "d, debug", "1: yes, 0: no", cxxopts::value<int>() )
		( "h, help", "print help" )
		;

	auto result = options.parse( argc, argv );

	if ( result.count( "help" ) )
	{
		std::cout << options.help() << std::endl;
		exit( 0 );
	}

	std::string input;
	std::string output;
	int imDim;
	int buggary;

	if ( result.count( "input" ) )
		input = result["input"].as<std::string>();

	if ( result.count( "output" ) )
		output = result["output"].as<std::string>();

	if ( result.count( "crop" ) )
		imDim = result["crop"].as<int>();

	if ( result.count( "debug" ) )
		buggary = result["debug"].as<int>();

	std::filesystem::path inputDir( input );
	std::filesystem::path outputDir( output );

	if ( std::filesystem::create_directory( outputDir ) ) {
		std::cout << output << " Directory created" << "\n";
	}

	runOnDir( input, inputDir, output, outputDir, imDim, buggary );
}
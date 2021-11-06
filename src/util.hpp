#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <filesystem>

void todo() {
    std::cout << "Reached TODO code. Exiting." << std::endl;
    exit(-1);
}

namespace twitls {
	namespace vec_ops {


        template<typename Vector>
        [[maybe_unused]] auto split_vector( const Vector& v, unsigned num_lines )
		{
			using Iterator = typename Vector::const_iterator;
			std::vector<Vector> rtn;
			Iterator it = v.cbegin();
			const Iterator end = v.cend();
            for (auto & i : v) {
                todo();
            }


//          This code below is dangerous and should never run. It needs to be reimplemented above.

/*			while ( it != :end ) {
                std::back_insert_iterator<Vector> inserter( v );
                const auto num_to_copy = std::min( static_cast<unsigned>(
                                                           std::distance( it, end ) ), num_lines );
                std::copy( it, it + num_to_copy, inserter );
                rtn.push_back( std::move( v ) );
                std::advance( it, num_to_copy );
            }
*/
			return rtn;
		}

        [[maybe_unused]] void print( std::vector<std::string> const& vecInput )
		{
			std::copy( vecInput.begin(), vecInput.end(), std::ostream_iterator<std::string>( std::cout, " " ) );
		}
	}
	namespace imgExt {
		bool is_image_extension( std::string input )
		{
			// apparently this converts the string (input), to lowercase
			transform( input.begin(), input.end(), input.begin(), ::tolower );
			std::vector<std::string> valid_extensions = { ".jpg", ".jpeg", ".png", ".webp", ".tif", ".tiff" };
			for ( auto& i : valid_extensions ) {
				if ( input == i ) {
					return true;
				}
			}
			return false;
		}
	}
	namespace count {
        [[maybe_unused]] int countfiles( std::string& directoryCount )
		{
			int numFiles = 0;
			for ( const auto& entry : std::filesystem::directory_iterator( directoryCount ) ) {
				if ( entry.is_regular_file() ) {
					numFiles++;
				}
			}
			return numFiles;
		}
		auto diff( int dimension, int tileSize )
		{
			return ( static_cast<double>( dimension ) + tileSize ) / tileSize;
		}
		auto padd( int difference, int tileSize )
		{
			return static_cast<double>(( difference ) - 1 ) * tileSize;
		}
		auto divv( int dimension, int padding )
		{
			return  static_cast<double>( dimension ) - padding;
		}

        [[maybe_unused]] auto isDiff( int W, int H )
		{
			if ( W <= H ) {
				return H;
			}
			else {
				return W;
			}
		}
	}
	namespace randgen {
        [[maybe_unused]] std::string randomNameGen( const int length )
		{
			std::string randString;
			static const char alphanum[] =
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"0123456789";
			std::random_device rd;
			std::mt19937 generator( rd() );
			randString.reserve( length );
			for ( int i = 0; i < length; ++i ) {
				randString += alphanum[generator() % ( sizeof( alphanum ) - 1 )];
			}
			return randString;
		}

        [[maybe_unused]] int randomNumber( const int low, const int high )
		{
			std::random_device rd;
			std::mt19937 generator( rd() );
			std::uniform_int_distribution r01( low, high );
			int rNum = r01( generator );
			return rNum;
		}

        [[maybe_unused]] std::string pickSampling()
		{
			srand( (unsigned int)time( nullptr ) );
			const std::string YUVsample[3] = {  "yuv422p",
												"yuv420p",
												"yuv410p" };
			return YUVsample[rand() % 3];
		}

        [[maybe_unused]] std::string pickCompressor()
		{
			srand( (unsigned int)time( nullptr ) );
			const std::string compressor[3] = { "libx264",
												"libx265",
												"libvpx-vp9" };
			return compressor[rand() % 3];
		}
	}
	namespace scriptmake {
        [[maybe_unused]] std::string ffmpegScriptShell( const std::string& directory, int leading_zeros, const std::string& fps,
									   const std::string& codec, const std::string& pixelfmt,
									   const std::string& CRF, const std::string& video_name )
		{
			std::string ffmpeg_lead = "%0" + std::to_string( leading_zeros ) + "d";
			std::string ffmpegCall( "ffmpeg" );
			std::string fpsCall = " -framerate " + fps + " ";
			std::string codecCall = " -c:v " + codec;
			std::string pixFmtCall = " -pix_fmt " + pixelfmt;
			std::string crfCall = " -crf " + CRF;
			std::string x265_log_level;
			if ( codec == "libx265" ) {
				x265_log_level = " -x265-params log-level=-1";
			}
			else {
				x265_log_level = "";
			}

			std::string frName( "montage_frame_" );
			std::string full_string_for_script;
			full_string_for_script += ffmpegCall;
			full_string_for_script += " -loglevel quiet";
			full_string_for_script += fpsCall;
			full_string_for_script += "-i ";
			full_string_for_script += directory;
			full_string_for_script += frName;
			full_string_for_script += ffmpeg_lead;
			full_string_for_script += ".png";
			full_string_for_script += pixFmtCall;
			full_string_for_script += codecCall;
			full_string_for_script += x265_log_level;
			full_string_for_script += crfCall;
			full_string_for_script += " ";
			full_string_for_script += video_name;
			return full_string_for_script;
		}

        [[maybe_unused]] std::string ffmpegScriptBat( const std::string& directory, int leading_zeros, const std::string& fps,
									 const std::string& codec, const std::string& pixelfmt,
									 const std::string& CRF, const std::string& video_name )
		{
			std::string ffmpeg_lead = "%0" + std::to_string( leading_zeros ) + "d";
			std::string ffmpegCall( "ffmpeg" );
			std::string fpsCall = " -framerate " + fps + " ";
			std::string codecCall = " -c:v " + codec;
			std::string pixFmtCall = " -pix_fmt " + pixelfmt;
			std::string crfCall = " -crf " + CRF;
			std::string x265_log_level;
			if ( codec == "libx265" ) {
				x265_log_level = " -x265-params log-level=-1";
			}
			else {
				x265_log_level = "";
			}

			std::string frName( "montage_frame_" );
			std::string full_string_for_script;
			full_string_for_script += ffmpegCall;
			full_string_for_script += " -loglevel quiet";
			full_string_for_script += fpsCall;
			full_string_for_script += "-i ";
			full_string_for_script += '"';
			full_string_for_script += directory;
			full_string_for_script += frName;
			full_string_for_script += ffmpeg_lead;
			full_string_for_script += ".png";
			full_string_for_script += '"';
			full_string_for_script += pixFmtCall;
			full_string_for_script += codecCall;
			full_string_for_script += x265_log_level;
			full_string_for_script += crfCall;
			full_string_for_script += " ";
			full_string_for_script += video_name;
			return full_string_for_script;
		}

        [[maybe_unused]] std::string extractScriptShell( const std::string& input_Video, int leading_zeros, const std::string& outputDirectory )
		{
			std::string ffmpeg_lead = "%0" + std::to_string( leading_zeros ) + "d";
			std::string ffmpegCall( "ffmpeg" );

			std::string frName( "montage_frame_" );
			std::string full_extract_string;
			full_extract_string += ffmpegCall;
			full_extract_string += " -loglevel quiet";
			full_extract_string += " -i ";
			full_extract_string += input_Video;
			full_extract_string += " ";
			full_extract_string += outputDirectory;
			full_extract_string += frName;
			full_extract_string += ffmpeg_lead;
			full_extract_string += ".png";
			return full_extract_string;
		}

        [[maybe_unused]] std::string extractScriptBat( const std::string& input_Video, int leading_zeros, const std::string& outputDirectory )
		{
			std::string ffmpeg_lead = "%0" + std::to_string( leading_zeros ) + "d";
			std::string ffmpegCall( "ffmpeg" );

			std::string frName( "montage_frame_" );
			std::string full_extract_string;
			full_extract_string += ffmpegCall;
			full_extract_string += " -loglevel quiet";
			full_extract_string += " -i ";
			full_extract_string += input_Video;
			full_extract_string += " ";
			full_extract_string += '"';
			full_extract_string += outputDirectory;
			full_extract_string += frName;
			full_extract_string += ffmpeg_lead;
			full_extract_string += ".png";
			full_extract_string += '"';
			return full_extract_string;
		}
	}
}





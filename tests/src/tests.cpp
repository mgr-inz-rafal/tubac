/*
 *
 * Turbo Basic Compiler by mgr_inz_rafal.
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rchabowski@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 *														// mgr inz. Rafal
 * ----------------------------------------------------------------------------
 */

 // tests.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>

#include <boost/process.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>

#include "process_executor.h"
#include "process_group_executor.h"

#define CATCH_CONFIG_MAIN
#include "../../external/catch/catch.hpp"

#ifdef _WIN32
#define CATCH_CONFIG_COLOUR_WINDOWS
const std::string tubac_path = "../x64/Debug/020_TuBaC.exe";
const std::string mads_path = "tools/mads.exe";
const std::string atari_path = "tools/atari800/atari800.exe";
const std::string franny_path = "tools/franny/franny.exe";
#elif __linux
const std::string tubac_path = "../020_TuBaC/tubac";
const std::string mads_path = "tools/mads";
const std::string atari_path = "tools/atari800/atari800";
const std::string franny_path = "tools/franny/franny";
#endif
const std::string atari_DOS_path = "tools/atari800/DOS.atr";
const std::string atari_TBXL_path = "tools/atari800/TBXL.atr";
const std::string suites_path = "suites/";
const std::string test_tmp_dir = "tools/tmp";
const std::string test_tmp_source_name = "SOURCE.TXT";		// Uppercase to make Atari happy
const std::string test_tmp_source = "tmp/" + test_tmp_source_name;
const std::string test_tmp_asm = "tmp/source.asm";
const std::string test_tmp_bin = "tmp/source.xex";
const std::string test_tmp_image = "tmp/test.atr";

std::chrono::seconds atari_run_timeout(3);

namespace bp = boost::process;
namespace bf = boost::filesystem;
using namespace std::chrono_literals;

void remove_n_lines_from_stream(std::stringstream& stream, unsigned int n)
{
	std::string tmp;
	for (size_t i = 0; i < n; ++i)
	{
		std::getline(stream, tmp);
	}
}

// Skips first 3 lines that contain emulator introduction
// and then reads until the end of the string. Remove single
// endline from the end of the string before returning.
// TODO: Do not hardcode 3
std::string parse_atari_binary_test(const std::string& str)
{
	std::stringstream stream(str);
	remove_n_lines_from_stream(stream, 3);
	std::string tmp;
	std::stringstream ret;
	while (std::getline(stream, tmp))
	{
		ret << tmp << std::endl;
	}
	tmp = ret.str();
	boost::trim(tmp);
	return tmp;
}

// Skips first 32 lines that contain emulator introduction
// and then reads until the first "READY". Remove double
// endline from the end of the string before returning.
// TODO: Do not hardcode 32
std::string parse_atari_listing_test(const std::string& str)
{
	std::stringstream stream(str);
	remove_n_lines_from_stream(stream, 32);
	std::string tmp;
	std::stringstream ret;
	while (std::getline(stream, tmp))
	{
		if(std::string::npos == tmp.find("READY"))
		{
			ret << tmp << std::endl;
		}
		else
		{
			break;
		}
	}
	tmp = ret.str();
	boost::trim(tmp);
	return tmp;
}

// Executes the given TBXL listing on Atari twice.
// 1. By compiling with Tubac->Mads and running .xex
// 2. By creating .atr disk and using TBXL to parse the program
// Returns parsed output from both machines
std::pair<std::string, std::string> execute_on_atari(std::string test_program)
{
	try
	{
		test_program += '\n';

		// Delete tmp files
		bf::remove(test_tmp_image);
		bf::remove(test_tmp_bin);
		bf::remove(test_tmp_source);
		bf::remove(test_tmp_asm);

		// Write test file
		std::ofstream out(test_tmp_source, std::ios::binary);
		out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		out << test_program;
		out.close();

		process_executor pr_tubac(
			tubac_path,
			{
				"--number-type=integer",
				(boost::format("--output-file=%1%") % test_tmp_asm).str(),
				test_tmp_source
			});

		process_executor pr_mads(
			mads_path,
			{
				test_tmp_asm,
				(boost::format("-o:%1%") % test_tmp_bin).str(),
				"-x"
			});

		process_executor pr_atari_binary(
			atari_path,
			{
				test_tmp_bin,
				"-turbo"
			},
			2s);

		process_executor pr_franny_create_image(
			franny_path,
			{
				"-C",
				test_tmp_image,
				"-f",				// AtariDOS
				"a",
				"-t",				// Single density
				"s" }
		);

		process_executor pr_franny_add_listing(
			franny_path,
			{
				test_tmp_image,
				"-A",
				"-i",
				test_tmp_source,
				"-o",
				test_tmp_source_name,
				"-c"				// Convert endlines to Atari
			}
		);

		process_executor pr_atari_listing(
			atari_path,
			{
				atari_DOS_path,
				atari_TBXL_path,
				test_tmp_image,
				"-turbo"
			},
			2s);
		pr_atari_listing
			<< "L\n"
			<< "D2:AUTORUN.SYS\n"
			<< (boost::format("ENTER \"D3:%1%\"\n") % test_tmp_source_name).str()
			<< "RUN\n";

#ifdef _WIN32
		std::optional<std::string> result_binary_test;
		std::optional<std::string> result_listing_test;
#else
		std::experimental::optional<std::string> result_binary_test;
		std::experimental::optional<std::string> result_listing_test;
#endif
		std::thread thread_binary_test([&]()
		{
			process_group_executor group({
				&pr_tubac,
				&pr_mads,
				&pr_atari_binary });
			result_binary_test = group.run();
		});
		std::thread thread_listing_test([&]()
		{
			process_group_executor group({
				&pr_franny_create_image,
				&pr_franny_add_listing,
				&pr_atari_listing });
			result_listing_test = group.run();
		});
		thread_binary_test.join();
		thread_listing_test.join();

		return std::make_pair(parse_atari_binary_test(result_binary_test.value()), parse_atari_listing_test(result_listing_test.value()));
	}
#ifdef _WIN32
	catch(const std::bad_optional_access)
	{
		std::cout << "CHILD ERROR: Not all test processess finished correctly" << std::endl;
		return std::make_pair("Exception", "caught");
	}
#endif
	catch(const std::ifstream::failure& e)
	{
		std::cout << "FILE ACCESS ERROR: " << e.what() << std::endl;
		return std::make_pair("Exception", "caught");
	}
	catch(const std::exception& e)
	{
		std::cout << "GENERAL ERROR: " << e.what() << std::endl;
		return std::make_pair("Exception", "caught");
	}
}

std::string atarize_listing(std::string listing)
{
	listing.erase(std::remove(listing.begin(), listing.end(), '\t'), listing.end());
	return listing;
}

std::string content_of_file(const std::string& name)
{
	auto ifs = std::ifstream(name);
	return {(std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()};
}

#define RUN_AND_CHECK(FILENAME, LISTING) \
		{ \
		std::string listing = atarize_listing(LISTING); \
		INFO(listing) \
		INFO(FILENAME) \
		result = execute_on_atari(listing); \
		CHECK(result.first == result.second); \
		}

std::pair<std::string, std::string> result;

TEST_CASE("Turbo Basic Compiler") {
	std::vector<bf::path> suites = { bf::directory_iterator(suites_path), {} };
	for (auto& suite : suites)
	{
		SECTION(suite.string())
		{
			std::vector<bf::path> tests = { bf::directory_iterator(suite.string()), {} };
			for (auto& test : tests)
			{
				auto ifs = std::ifstream(test.string());
				RUN_AND_CHECK(
					test.string(),
					{(std::istreambuf_iterator<char>(ifs)) BOOST_PP_COMMA() std::istreambuf_iterator<char>()});
			}
		}
	}
}

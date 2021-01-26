#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <cinttypes>
#ifdef _WIN64
#include <experimental/filesystem>
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
#include <filesystem>
#endif

#if defined (_DEBUG) || defined (DEBUG)
#include "ld/LeakDetector.hpp"
#endif

#include <memory>
#include <cmath>
#include <stdio.h>

#include "gd.h"
#include "formatData.h"
#include "drawMap.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

void drawTopTribes(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes) {

	gdImagePtr image;
	FILE* topTribeOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribe.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribe.png";
#endif

	topTribeOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);
				

				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}


	for (uint32_t i = 0; i < (uint32_t)((float)kLength * worldLength); i += kLength) {

		uint32_t check = (uint32_t)((float)kLength * partialK) + i;

		gdImageLine(image, check, 30, check, 1030, kNumColor);
		gdImageLine(image, 0, check + 30, 1000, check + 30, kNumColor);

	}

	int rect[8];
	//	meant for gdImageStringFTEx for unicode, but cannot get to work

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFreeFontCache();

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 96;
	gdStringExtras.vdpi = 96;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topTribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topTribes.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOut);

	fclose(topTribeOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayers(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers) {

	gdImagePtr image;
	FILE* topPlayerOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayer.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayer.png";
#endif

	topPlayerOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

				x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
				y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

				x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
				y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;
	

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topPlayers.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOut);

	fclose(topPlayerOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeOD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODTribes) {

	gdImagePtr image;
	FILE* topTribeOdaOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribeOD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribeOD.png";
#endif
	topTribeOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODTribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODTribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topODTribes.at(i)->getOD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top OD Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOdaOut);

	fclose(topTribeOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeODA(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODATribes) {

	gdImagePtr image;
	FILE* topTribeOdaOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribeODA.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribeODA.png";
#endif
	topTribeOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODATribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODATribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODATribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODATribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODATribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODATribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODATribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODATribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = converter.to_bytes(topODATribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = RecursiveCommas(topODATribes.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODATribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top ODA Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOdaOut);

	fclose(topTribeOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeODD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODDTribes) {

	gdImagePtr image;
	FILE* topTribeOddOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribeODD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribeODD.png";
#endif
	topTribeOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODDTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODDTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topODDTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODDTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDTribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDTribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = converter.to_bytes(topODDTribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = RecursiveCommas(topODDTribes.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODDTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top ODD Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOddOut);

	fclose(topTribeOddOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerOD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODPlayers) {

	gdImagePtr image;
	FILE* topPlayerOdaOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayerOD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayerOD.png";
#endif
	topPlayerOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topODPlayers.at(i)->getOD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top OD Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOdaOut);

	fclose(topPlayerOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerODA(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODAPlayers) {

	gdImagePtr image;
	FILE* topPlayerOdaOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayerODA.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayerODA.png";
#endif
	topPlayerOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODAPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODAPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODAPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODAPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODAPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODAPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODAPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODAPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODAPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODAPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topODAPlayers.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODAPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top ODA Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOdaOut);

	fclose(topPlayerOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerODD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODDPlayers) {

	gdImagePtr image;
	FILE* topPlayerOddOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayerODD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayerODD.png";
#endif
	topPlayerOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODDPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODDPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODDPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topODDPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODDPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODDPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topODDPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topODDPlayers.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topODDPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top ODD Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOddOut);

	fclose(topPlayerOddOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeConqs(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topConqTribes) {

	gdImagePtr image;
	FILE* topTribeConqOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribeConq.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribeConq.png";
#endif
	topTribeConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51),
			charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
			 gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topConqTribes.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqTribes.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqTribes.at(i)->getConq(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topConqTribes.at(i)->getConqCount(); j++) {

				x = std::get<0>(topConqTribes.at(i)->getConq(j)->getCoord());
				y = std::get<1>(topConqTribes.at(i)->getConq(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqTribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqTribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topConqTribes.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topConqTribes.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Nobling Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeConqOut);

	fclose(topTribeConqOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeLosses(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topLossTribes) {

	gdImagePtr image;
	FILE* topTribeLossOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topTribeLoss.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topTribeLoss.png";
#endif
	topTribeLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51),
			charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topLossTribes.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossTribes.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossTribes.at(i)->getLoss(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossTribes.at(i))
			continue;

		for (uint32_t j = 0; j < topLossTribes.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossTribes.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossTribes.at(i)->getLoss(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossTribes[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossTribes[i]->getTag());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topLossTribes.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topLossTribes.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Nobled Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeLossOut);

	fclose(topTribeLossOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerConqs(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topConqPlayers) {

	gdImagePtr image;
	FILE* topPlayerConqOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayerConq.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayerConq.png";
#endif
	topPlayerConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topConqPlayers.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqPlayers.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqPlayers.at(i)->getConq(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topConqPlayers.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqPlayers.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqPlayers.at(i)->getConq(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topConqPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topConqPlayers.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topConqPlayers.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Nobling Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerConqOut);

	fclose(topPlayerConqOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerLosses(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topLossPlayers) {

	gdImagePtr image;
	FILE* topPlayerLossOut;
	std::string filePath = "";

	std::string fontPath = "./deps/arialuni.ttf";
#ifdef _WIN64
	filePath = "./maps/" + world + "/topPlayerLoss.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath = "/var/www/twaumm/maps/" + world + "/topPlayerLoss.png";
#endif
	topPlayerLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topLossPlayers.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossPlayers.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossPlayers.at(i)->getLoss(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossPlayers.at(i))
			continue;

		for (uint32_t j = 0; j < topLossPlayers.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossPlayers.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossPlayers.at(i)->getLoss(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);

		}


	}

		gdFTStringExtra gdStringExtras;
		gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
		gdStringExtras.linespacing = 1.05;
		gdStringExtras.charmap = gdFTEX_Unicode;
		gdStringExtras.hdpi = 0;
		gdStringExtras.vdpi = 0;
		gdStringExtras.xshow = nullptr;
		gdStringExtras.fontpath = nullptr;

	for (uint32_t i = 0; i < 15; i++) {

		if (!topLossPlayers[i])
			continue;

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossPlayers[i]->getName());
		gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

		data = RecursiveCommas(topLossPlayers.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = RecursiveCommas(topLossPlayers.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, "Top Nobled Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerLossOut);

	fclose(topPlayerLossOut);
	gdImageDestroy(image);

}
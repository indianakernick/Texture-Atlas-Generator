//
//  search dir.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "search dir.hpp"

#include <dirent.h>
#include "logger.hpp"

DirSearchError::DirSearchError()
  : std::runtime_error("Failed to search for files in this directory") {}

NoSupportedFilesError::NoSupportedFilesError()
  : std::runtime_error("There are no supported files in this directory") {}

bool extIsImage(const StringView ext) {
  static const StringView EXTS[] = {
    "jpg", "jpeg", "png", "bmp", "psd", "tga", "gif", "hdr", "pic", "pgm", "ppm"
  };
  
  return std::any_of(std::cbegin(EXTS), std::cend(EXTS), [ext](const StringView thisExt) {
    return ext == thisExt;
  });
}

StringView getExt(const StringView path) {
  const size_t lastDot = path.find_last_of('.');
  if (lastDot >= path.size() - 1) {
    return {"", 0};
  } else {
    return {
      path.data() + (lastDot + 1),
      path.size() - (lastDot + 1)
    };
  }
}

std::vector<std::string> findFiles(
  const std::string &path,
  const SearchPred pred
) {
  Logger::get() << "Searching directory \"" << path << "\"\n";
  
  std::vector<std::string> files;
  
  std::unique_ptr<DIR, int(*)(DIR *)> dir(opendir(path.c_str()), closedir);
  if (dir) {
    while (const dirent *entry = readdir(dir.get())) {
      const StringView ext = getExt(entry->d_name);
      if (ext.size() && pred(ext)) {
        files.emplace_back(path + '/' + entry->d_name);
      }
    }
  } else {
    throw DirSearchError();
  }
  
  if (files.size() == 0) {
    throw NoSupportedFilesError();
  }
  
  return files;
}

void findFilesRecHelper(
  const std::string &path,
  const SearchPred pred,
  const size_t maxDepth,
  std::vector<std::string> &files
) {
  if (maxDepth == 0) {
    return;
  }
  
  Logger::get() << "Searching directory \"" << path << "\"\n";
  
  std::unique_ptr<DIR, int(*)(DIR *)> dir(opendir(path.c_str()), closedir);
  if (dir) {
    while (const dirent *entry = readdir(dir.get())) {
      if (entry->d_type == DT_DIR) {
        findFilesRecHelper(path, pred, maxDepth - 1, files);
      } else {
        const StringView ext = getExt(entry->d_name);
        if (ext.size() && pred(ext)) {
          files.emplace_back(path + '/' + entry->d_name);
        }
      }
    }
  } else {
    throw DirSearchError();
  }
}

std::vector<std::string> findFilesRec(
  const std::string &path,
  const SearchPred pred,
  const size_t maxDepth
) {
  
  Logger::get() << "Searching directory \"" << path << "\"\n";
  
  std::vector<std::string> files;
  
  findFilesRecHelper(path, pred, maxDepth, files);
  
  if (files.size() == 0) {
    throw NoSupportedFilesError();
  }
  
  return files;
}

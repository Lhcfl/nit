#include "lib/nit_common.hpp"
#include <string>
#include <vector>

#ifndef __H_NIT_MODEL_STATUS
#define __H_NIT_MODEL_STATUS

class NitStatusModel {

public:
  enum class FileStatus {
    UNTRACKED = -1,
    UNCHANGED = 0,
    MODIFIED = 1,
    NEWFILE = 2,
    REMOVED = 3,
  };

  struct FileStatusNamePair {
    std::string filename;
    FileStatus status;

    FileStatusNamePair(const std::string &filename, FileStatus status)
        : filename(filename), status(status) {}
    FileStatusNamePair(std::string &&filename, FileStatus status)
        : filename(filename), status(status) {}
  };

  /**
   * working dir diff to staging area
   */
  std::vector<FileStatusNamePair> workingDirStatus;
  /**
   * staging area diff to last commit
   */
  std::vector<FileStatusNamePair> stagingAreaStatus;

  std::string calculateStatus() {
    NitLogger changesToBeCommited("", "", "");
    NitLogger changesNotStaged("", "", "");
    NitLogger untracked("", "", "");
    NitLogger tmpLogger("", "", "");

    const std::string TAB = "\t";

    for (const auto &fp : workingDirStatus) {
      switch (fp.status) {
      case FileStatus::UNTRACKED:
        untracked.log(TAB, NitColor::green("untracked: " + fp.filename));
        break;
      case FileStatus::REMOVED:
        changesNotStaged.log(TAB, NitColor::red("removed: " + fp.filename));
        break;
      case FileStatus::MODIFIED:
        changesNotStaged.log(TAB, NitColor::yellow("modified: " + fp.filename));
      case FileStatus::UNCHANGED:
        // We don't want to log unchanged files
        break;
      case FileStatus::NEWFILE:
        throw NitError("Unexpected new file here");
      }
    }

    for (const auto &fp : stagingAreaStatus) {
      switch (fp.status) {
      case FileStatus::UNTRACKED:
        throw NitError("Unexpected untracked File in staging area");
      case FileStatus::REMOVED:
        changesToBeCommited.log(TAB, NitColor::red("removed: " + fp.filename));
        break;
      case FileStatus::MODIFIED:
        changesToBeCommited.log(TAB,
                                NitColor::yellow("modified: " + fp.filename));
      case FileStatus::UNCHANGED:
        // We don't want to log unchanged files
        break;
      case FileStatus::NEWFILE:
        changesToBeCommited.log(TAB,
                                NitColor::green("new file: " + fp.filename));
        break;
      }
    }

    auto &&strChangesToBeCommited = changesToBeCommited.getStream().str();
    auto &&strChangesNotStaged = changesNotStaged.getStream().str();
    auto &&strUntracked = untracked.getStream().str();

    if (!strChangesToBeCommited.empty()) {
      tmpLogger.log("Changes to be committed:");
      tmpLogger.log(strChangesToBeCommited);
      tmpLogger.newLine();
    }
    if (!strChangesNotStaged.empty()) {
      tmpLogger.log("Changes not staged for commit:");
      tmpLogger.log(strChangesNotStaged);
      tmpLogger.newLine();
    }
    if (!strUntracked.empty()) {
      tmpLogger.log("Untracked files:");
      tmpLogger.log(
          "  (use \"nit add <file>...\" to include in what will be committed)");
      tmpLogger.log(strUntracked);
      tmpLogger.newLine();
    }
    if (tmpLogger.getStream().str().empty()) {
      tmpLogger.log("Nothing to commit, working tree clean");
      tmpLogger.newLine();
    }

    return tmpLogger.getStream().str();
  }
};

#endif
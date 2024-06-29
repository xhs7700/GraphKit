#include "graphkit.h"
#include "graphkitutils.h"
#include <archive.h>
#include <archive_entry.h>
#include <cpr/callback.h>
#include <cpr/session.h>
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <string>

namespace gkit {
namespace fs = std::filesystem;

void DownloadFile(const std::string& url, std::ofstream& fout)
{
    cpr::Session session;
    session.SetUrl(url);
    SetSpinner spinner(fmt::format("Downloading archive {}...", fs::path(url).filename().string()), session.GetDownloadFileLength());
    session.SetProgressCallback(cpr::ProgressCallback([&](cpr::cpr_off_t downloadTotal, cpr::cpr_off_t downloadNow, cpr::cpr_off_t, cpr::cpr_off_t, intptr_t) {
        spinner.setProgress(downloadNow);
        return true;
    }));
    session.Download(fout);
    spinner.markAsCompleted();
}

void ExtractFile(const fs::path& path, std::function<int(archive*)> formatFunc)
{
    archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    formatFunc(a);
    if (archive_read_open_filename(a, path.c_str(), 10240) != ARCHIVE_OK) {
        std::string errStr = fmt::format("Failed to open archive: {}\n", archive_error_string(a));
        std::cerr << errStr;
        std::terminate();
    }
    archive_entry* entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        const char* pathName = archive_entry_pathname(entry);
        const fs::path filePath = path.parent_path() / pathName;
        if (archive_entry_filetype(entry) == AE_IFDIR)
            fs::create_directory(filePath);
        else {
            std::ofstream fout(filePath, std::ios::binary);
            if (fout) {
                char buf[8192];
                la_ssize_t size;
                while ((size = archive_read_data(a, buf, sizeof(buf))) > 0)
                    fout.write(buf, static_cast<std::streamsize>(size));
            } else {
                std::string errStr = fmt::format("Failed to create file: {}\n", pathName);
                std::cerr << errStr;
                std::terminate();
            }
        }
    }
}

fs::path SearchKonectPath(const fs::path& dirPath)
{
    if (!fs::exists(dirPath))
        return {};
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        const fs::path entryPath = entry.path();
        if (fs::is_regular_file(entryPath) && entryPath.filename().string().starts_with("out."))
            return entryPath;
    }
    return {};
}

fs::path GetKonectPath(const std::string& internalName)
{
    const fs::path tmpDir = std::filesystem::temp_directory_path();
    const fs::path dirPath = tmpDir / internalName;
    const fs::path filePath = SearchKonectPath(dirPath);
    if (fs::exists(filePath))
        return filePath;
    else if (fs::exists(dirPath))
        fs::remove_all(dirPath);
    const fs::path bzPath = tmpDir / fmt::format("{}.tar.bz2", internalName);
    if (!fs::exists(bzPath)) {
        std::string url = fmt::format("http://konect.cc/files/download.tsv.{}.tar.bz2", internalName);
        std::ofstream fout(bzPath, std::ios::binary);
        DownloadFile(url, fout);
    }
    ExtractFile(bzPath, archive_read_support_format_all);
    return SearchKonectPath(dirPath);
}

fs::path GetSnapPath(const std::string& url)
{
    const fs::path tmpDir = std::filesystem::temp_directory_path();
    const fs::path urlPath(url);
    const fs::path filePath = tmpDir / urlPath.stem();
    if (fs::exists(filePath))
        return filePath;
    const fs::path gzPath = tmpDir / urlPath.filename();
    if (!fs::exists(gzPath)) {
        std::ofstream fout(gzPath, std::ios::binary);
        DownloadFile(url, fout);
    }
    ExtractFile(gzPath, archive_read_support_format_raw);
    return filePath;
}
}

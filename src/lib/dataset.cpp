#include "graphkit.h"
#include "utils.h"
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
#include <utility>

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
    fmt::println("bzPath = {}", path.c_str());
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

fs::path GetKonectPath(const std::string& internalName)
{
    const fs::path tmpDir = std::filesystem::temp_directory_path();
    const fs::path filePath = tmpDir / internalName / fmt::format("out.{}", internalName);
    if (fs::exists(filePath))
        return filePath;
    const fs::path bzPath = tmpDir / fmt::format("{}.tar.bz2", internalName);
    if (!fs::exists(bzPath)) {
        std::string url = fmt::format("http://konect.cc/files/download.tsv.{}.tar.bz2", internalName);
        std::ofstream fout(bzPath, std::ios::binary);
        DownloadFile(url, fout);
    }
    ExtractFile(bzPath, archive_read_support_format_all);
    return filePath;
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

UnweightedUndiGraph LoadUnweightedUndiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return UnweightedUndiGraph(std::move(name), source);
}
UnweightedDiGraph LoadUnweightedDiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return UnweightedDiGraph(std::move(name), source);
}
UnweightedUndiGraph LoadUnweightedUndiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadUnweightedUndiKonect(internalName, std::move(name));
}
UnweightedDiGraph LoadUnweightedDiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadUnweightedDiKonect(internalName, std::move(name));
}
WeightedUndiGraph LoadWeightedUndiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return WeightedUndiGraph(std::move(name), source);
}
WeightedDiGraph LoadWeightedDiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return WeightedDiGraph(std::move(name), source);
}
WeightedUndiGraph LoadWeightedUndiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadWeightedUndiKonect(internalName, std::move(name));
}
WeightedDiGraph LoadWeightedDiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadWeightedDiKonect(internalName, std::move(name));
}

SimpleUndiGraph LoadSimpleUndiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return SimpleUndiGraph(std::move(name), source);
}
SimpleDiGraph LoadSimpleDiKonect(std::string& internalName, std::string&& name)
{
    const fs::path source = GetKonectPath(internalName);
    return SimpleDiGraph(std::move(name), source);
}
SimpleUndiGraph LoadSimpleUndiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadSimpleUndiKonect(internalName, std::move(name));
}
SimpleDiGraph LoadSimpleDiKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadSimpleDiKonect(internalName, std::move(name));
}

UnweightedUndiGraph LoadUnweightedUndiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return UnweightedUndiGraph(std::move(name), source);
}
WeightedUndiGraph LoadWeightedUndiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return WeightedUndiGraph(std::move(name), source);
}
SimpleUndiGraph LoadSimpleUndiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return SimpleUndiGraph(std::move(name), source);
}
UnweightedDiGraph LoadUnweightedDiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return UnweightedDiGraph(std::move(name), source);
}
WeightedDiGraph LoadWeightedDiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return WeightedDiGraph(std::move(name), source);
}
SimpleDiGraph LoadSimpleDiSnap(std::string& url, std::string&& name)
{
    const fs::path source = GetSnapPath(url);
    return SimpleDiGraph(std::move(name), source);
}
}

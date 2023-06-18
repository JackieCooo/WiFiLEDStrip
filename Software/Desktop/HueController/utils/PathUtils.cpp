#include "PathUtils.h"

QString PathUtils::get(const QString &str) {
    QString path;
    char* cwd = _getcwd(nullptr, 0);

    path += cwd;
    path.replace("\\", "/");
    path += str;

    return path;
}

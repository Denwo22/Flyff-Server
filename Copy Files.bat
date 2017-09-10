@echo off
COPY "Output\AccountServer\Release\AccountServer.exe" "Program\AccountServer.exe"
COPY "Output\CacheServer\Release\CacheServer.exe" "Program\CacheServer.exe"
COPY "Output\Certifier\Release\Certifier.exe" "Program\Certifier.exe"
COPY "Output\CoreServer\Release\CoreServer.exe" "Program\CoreServer.exe"
COPY "Output\DatabaseServer\Release\DatabaseServer.exe" "Program\DatabaseServer.exe"
COPY "Output\LoginServer\Release\LoginServer.exe" "Program\LoginServer.exe"
COPY "Output\WorldServer\Release\WorldServer.exe" "Program\WorldServer.exe"

COPY "Output\Neuz\NoGameguard\Neuz.exe" "Resource\Neuz.exe"
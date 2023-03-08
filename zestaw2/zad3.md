# Zadanie 3
Przejrzyj dokumentację netcata, upewnij się co do znaczenia opcji -v, -l oraz -u. Sprawdź też co robi opcja -C, czyli --crlf. W jakich sytuacjach może ona być potrzebna?

## -v

Specify verbose output.

## -l

Listen for an incoming connection rather than initiate a connection to a remote host.

It is an error to use this option in conjunction with the –s or –z options.

If the –l option is used with a wildcard socket (no IP address or hostname specified) and without the –4 /–6 options, it accepts both IPv4 and IPv6 connections

## -u
Use UDP instead of the default option of TCP.

## -c
This option tells Ncat to convert LF line endings to CRLF
when taking input from standard input.  This is useful for
talking to some stringent servers directly from a terminal in
one of the many common plain-text protocols that use CRLF for
end-of-line.

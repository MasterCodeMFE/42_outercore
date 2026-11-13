#!/usr/bin/env bash

set -u

PROJECT_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
FT_LS="$PROJECT_ROOT/ft_ls"
SYSTEM_LS=$(command -v ls || true)

if [[ ! -x "$FT_LS" ]]; then
	printf 'No encuentro el ejecutable: %s\n' "$FT_LS" >&2
	printf 'Compila primero el proyecto con: make -C "%s"\n' "$PROJECT_ROOT" >&2
	exit 2
fi

if [[ -z "$SYSTEM_LS" ]]; then
	printf 'No encuentro el comando ls del sistema en PATH.\n' >&2
	exit 2
fi

TMP_ROOT=$(mktemp -d "${TMPDIR:-/tmp}/ft_ls_mandatory.XXXXXX") || exit 2
FIXTURE="$TMP_ROOT/fixture"
OUT_FT="$TMP_ROOT/ft_ls.stdout"
ERR_FT="$TMP_ROOT/ft_ls.stderr"
OUT_LS="$TMP_ROOT/ls.stdout"
ERR_LS="$TMP_ROOT/ls.stderr"
ERR_FT_NORMALIZED="$TMP_ROOT/ft_ls.stderr.normalized"
ERR_LS_NORMALIZED="$TMP_ROOT/ls.stderr.normalized"

export LC_ALL=C
export LANG=C
export TZ=UTC

if [[ -n "${FORCE_COLOR:-}" ]] \
	|| { [[ -t 1 ]] && [[ -z "${NO_COLOR:-}" ]] \
		&& [[ "${TERM:-dumb}" != "dumb" ]]; }; then
	C_RESET=$'\033[0m'
	C_BOLD=$'\033[1m'
	C_CYAN=$'\033[1;36m'
	C_BLUE=$'\033[1;34m'
	C_GREEN=$'\033[1;32m'
	C_RED=$'\033[1;31m'
	C_YELLOW=$'\033[1;33m'
else
	C_RESET=''
	C_BOLD=''
	C_CYAN=''
	C_BLUE=''
	C_GREEN=''
	C_RED=''
	C_YELLOW=''
fi

total=0
passed=0
failed=0
skipped=0
future_timestamp_supported=0
max_filename_supported=0
sparse_file_supported=0
extreme_timestamps_supported=0
symlink_chain_supported=0
max_symlink_supported=0
nanosecond_sort_supported=0
max_name=''
oversized_component=''
long_symlink_target=''
path_too_long=''
path_too_long_supported=0

cleanup()
{
	if [[ -n "${TMP_ROOT:-}" && -d "$TMP_ROOT" ]]; then
		chmod 700 "$FIXTURE/no_search" "$FIXTURE/no_access" \
			"$FIXTURE/recurse_root/no_access_child" 2>/dev/null || true
		rm -rf -- "$TMP_ROOT"
		if [[ ! -e "$TMP_ROOT" ]]; then
			printf '\nDatos temporales de las pruebas: borrados.\n'
		else
			printf '\nAVISO: no se pudo borrar el directorio temporal: %s\n' \
				"$TMP_ROOT" >&2
		fi
	fi
}

trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

pause_before()
{
	local title=$1
	local description=$2
	shift 2

	printf '\n============================================================\n'
	printf '%s%sPRUEBA:%s %s\n' "$C_BOLD" "$C_CYAN" "$C_RESET" "$title"
	printf '%sDESCRIPCION:%s %s\n' "$C_CYAN" "$C_RESET" "$description"
	if [[ "$#" -gt 0 ]]; then
		printf '%sCOMANDO ft_ls:%s ./ft_ls' "$C_BLUE" "$C_RESET"
		printf ' %q' "$@"
		printf '\n'
		printf '%sCOMANDO sistema:%s ls' "$C_BLUE" "$C_RESET"
		printf ' %q' "$@"
		printf '\n'
	else
		printf '%sCOMANDO ft_ls:%s ./ft_ls\n' "$C_BLUE" "$C_RESET"
		printf '%sCOMANDO sistema:%s ls\n' "$C_BLUE" "$C_RESET"
	fi
	printf '%sPulsa Enter para ejecutar esta prueba.%s\n' \
		"$C_BOLD" "$C_RESET"
	IFS= read -r _ || true
}

show_diff()
{
	local label=$1
	local expected=$2
	local actual=$3

	printf '\n--- Diferencias en %s (ls del sistema / ft_ls) ---\n' "$label"
	diff -u --label "ls: $label" --label "ft_ls: $label" \
		"$expected" "$actual" | sed -n 'l' || true
}

show_stream()
{
	local label=$1
	local file=$2

	printf '\n  --- %s ---\n' "$label"
	if [[ -s "$file" ]]; then
		sed -n 'l' "$file"
	else
		printf '  (vacío)\n'
	fi
}

run_compare()
{
	local label=$1
	shift
	local ours_status
	local system_status
	local test_fd_limit=${TEST_FD_LIMIT:-}
	local description='Se compararan stdout, stderr y codigo de salida de ft_ls y GNU ls.'
	local status_match=0
	local stdout_match=0
	local stderr_match=0
	local explanation
	local -a reasons

	total=$((total + 1))
	if [[ -n "$test_fd_limit" ]]; then
		description="$description Limite de archivos abiertos por proceso: $test_fd_limit."
		pause_before "$label" "$description" "$@"
		(
			ulimit -n "$test_fd_limit" || exit 125
			cd "$FIXTURE" || exit 125
			"$FT_LS" "$@"
		) >"$OUT_FT" 2>"$ERR_FT"
	else
		pause_before "$label" "$description" "$@"
		(
			cd "$FIXTURE" || exit 125
			"$FT_LS" "$@"
		) >"$OUT_FT" 2>"$ERR_FT"
	fi
	ours_status=$?

	if [[ -n "$test_fd_limit" ]]; then
		(
			ulimit -n "$test_fd_limit" || exit 125
			cd "$FIXTURE" || exit 125
			"$SYSTEM_LS" "$@"
		) >"$OUT_LS" 2>"$ERR_LS"
	else
		(
			cd "$FIXTURE" || exit 125
			"$SYSTEM_LS" "$@"
		) >"$OUT_LS" 2>"$ERR_LS"
	fi
	system_status=$?

	printf '\n  ========== SALIDA ft_ls (status %s) ==========\n' \
		"$ours_status"
	show_stream 'stdout' "$OUT_FT"
	show_stream 'stderr' "$ERR_FT"
	printf '\n  ========== SALIDA ls del sistema (status %s) ==========\n' \
		"$system_status"
	show_stream 'stdout' "$OUT_LS"
	show_stream 'stderr' "$ERR_LS"

	sed 's/^ft_ls:/ls:/' "$ERR_FT" >"$ERR_FT_NORMALIZED"
	cp "$ERR_LS" "$ERR_LS_NORMALIZED"

	if [[ "$ours_status" -eq "$system_status" ]]; then
		status_match=1
	fi
	if cmp -s "$OUT_FT" "$OUT_LS"; then
		stdout_match=1
	fi
	if cmp -s "$ERR_FT_NORMALIZED" "$ERR_LS_NORMALIZED"; then
		stderr_match=1
	fi
	if [[ "$status_match" -eq 1 && "$stdout_match" -eq 1 \
		&& "$stderr_match" -eq 1 ]]; then
		passed=$((passed + 1))
		printf '  %sPASS:%s "%s" paso porque stdout, stderr y el codigo %s coinciden con GNU ls.\n' \
			"$C_GREEN" "$C_RESET" "$label" "$ours_status"
	else
		failed=$((failed + 1))
		reasons=()
		if [[ "$status_match" -eq 0 ]]; then
			reasons+=("codigo de salida distinto: ft_ls=$ours_status, ls=$system_status")
		fi
		if [[ "$stdout_match" -eq 0 ]]; then
			reasons+=("stdout distinto: puede faltar o cambiar contenido, orden o formato")
		fi
		if [[ "$stderr_match" -eq 0 ]]; then
			reasons+=("stderr distinto: los diagnosticos no coinciden")
		fi
		printf -v explanation '%s; ' "${reasons[@]}"
		explanation=${explanation%; }
		printf '  %sFAIL:%s "%s" fallo: %s. El diff siguiente identifica las lineas distintas.\n' \
			"$C_RED" "$C_RESET" "$label" "$explanation"
		if ! cmp -s "$OUT_FT" "$OUT_LS"; then
			show_diff stdout "$OUT_LS" "$OUT_FT"
		fi
		if ! cmp -s "$ERR_FT_NORMALIZED" "$ERR_LS_NORMALIZED"; then
			show_diff stderr "$ERR_LS_NORMALIZED" "$ERR_FT_NORMALIZED"
		fi
	fi
}

run_compare_fd_limit()
{
	local limit=$1

	shift
	TEST_FD_LIMIT="$limit" run_compare "$@"
}

run_skip()
{
	local label=$1

	total=$((total + 1))
	pause_before "$label" "Se omite el caso: $2."
	skipped=$((skipped + 1))
	printf '  %sSKIP:%s "%s": %s\n' \
		"$C_YELLOW" "$C_RESET" "$label" "$2"
}

prepare_fixture()
{
	local local_deep_path bulk_name newline_name invalid_name tab_name
	local level index name_max long_component long_path chain_next
	local path_component

	mkdir -p "$FIXTURE/dirA/nested" "$FIXTURE/dirB" "$FIXTURE/empty"
	printf 'small file\n' >"$FIXTURE/plain.txt"
	: >"$FIXTURE/zero-byte"
	printf 'a second file\n' >"$FIXTURE/another.txt"
	printf 'a longer file name\n' >"$FIXTURE/very-long-filename.txt"
	printf 'hidden file\n' >"$FIXTURE/.hidden"
	printf 'nested hidden file\n' >"$FIXTURE/dirA/.nested-hidden"
	printf 'inside A\n' >"$FIXTURE/dirA/a.txt"
	printf 'inside A, second file\n' >"$FIXTURE/dirA/bigger-name.txt"
	printf 'inside nested directory\n' >"$FIXTURE/dirA/nested/deep.txt"
	printf 'inside B\n' >"$FIXTURE/dirB/b.txt"
	printf 'leading dash name\n' >"$FIXTURE/-leading-dash"
	printf 'leading space name\n' >"$FIXTURE/ leading-space.txt"
	printf 'trailing space name\n' >"$FIXTURE/trailing-space.txt "
	printf -v tab_name 'tab\tname.txt'
	printf 'embedded tab name\n' >"$FIXTURE/$tab_name"
	printf 'glob punctuation name\n' >"$FIXTURE/[glob]*?.txt"
	printf 'quote and backslash name\n' >"$FIXTURE/quote'and\\slash.txt"
	printf -v newline_name 'line\nbreak.txt'
	printf 'newline in name\n' >"$FIXTURE/$newline_name"
	printf -v invalid_name 'bad-\377-byte.txt'
	printf 'non-UTF-8 byte in name\n' >"$FIXTURE/$invalid_name"

	printf 'old timestamp\n' >"$FIXTURE/time-old.txt"
	printf 'new timestamp\n' >"$FIXTURE/time-new.txt"
	printf 'same timestamp, first name\n' >"$FIXTURE/tie-a.txt"
	printf 'same timestamp, second name\n' >"$FIXTURE/tie-b.txt"
	touch -t 202001010101.01 "$FIXTURE/time-old.txt"
	touch -t 202601020202.02 "$FIXTURE/time-new.txt"
	touch -t 202601030303.03 "$FIXTURE/tie-a.txt" "$FIXTURE/tie-b.txt"
	printf 'future timestamp\n' >"$FIXTURE/time-future.txt"
	if touch -d 'now + 30 minutes' "$FIXTURE/time-future.txt" 2>/dev/null; then
		future_timestamp_supported=1
	fi
	printf 'epoch timestamp\n' >"$FIXTURE/time-epoch.txt"
	printf '2038 boundary timestamp\n' >"$FIXTURE/time-2038.txt"
	if touch -d '@0' "$FIXTURE/time-epoch.txt" 2>/dev/null \
		&& touch -d '@2147483648' "$FIXTURE/time-2038.txt" 2>/dev/null \
		&& touch -d '@253402300799' "$FIXTURE/time-9999.txt" 2>/dev/null; then
		extreme_timestamps_supported=1
	fi
	mkdir "$FIXTURE/nano_times"
	printf 'nanosecond timestamp A\n' >"$FIXTURE/nano_times/nano-a"
	printf 'nanosecond timestamp B\n' >"$FIXTURE/nano_times/nano-b"
	printf 'nanosecond timestamp C\n' >"$FIXTURE/nano_times/nano-c"
	if command -v python3 >/dev/null 2>&1; then
		python3 - "$FIXTURE/nano_times" <<'PY'
import os
import sys
import time

directory = sys.argv[1]
base = (time.time_ns() // 1_000_000_000 - 60) * 1_000_000_000
for index, name in enumerate(("nano-a", "nano-b", "nano-c"), 1):
    timestamp = base + index * 100_000_000
    os.utime(os.path.join(directory, name), ns=(timestamp, timestamp))
PY
		nanosecond_sort_supported=1
	fi

	name_max=$(getconf NAME_MAX "$FIXTURE" 2>/dev/null || printf '0')
	if [[ "$name_max" -ge 255 ]]; then
		printf -v max_name '%0255d' 7
		printf 'maximum-length filename\n' >"$FIXTURE/$max_name"
		mkdir "$FIXTURE/max-name-directory"
		printf 'nested maximum-length filename\n' \
			>"$FIXTURE/max-name-directory/$max_name"
		max_filename_supported=1
	fi
	printf -v oversized_component '%0260d' 7
	path_too_long="$FIXTURE"
	printf -v path_component '%0100d' 3
	for ((level = 1; level <= 42; level++)); do
		path_too_long="$path_too_long/$path_component"
	done
	if [[ "${#path_too_long}" -gt 4096 ]]; then
		path_too_long_supported=1
	fi

	if command -v truncate >/dev/null 2>&1; then
		truncate -s 4294967297 "$FIXTURE/sparse-over-4g.bin"
		sparse_file_supported=1
	elif command -v python3 >/dev/null 2>&1; then
		python3 - "$FIXTURE/sparse-over-4g.bin" <<'PY'
import os
import sys

with open(sys.argv[1], "wb") as file:
    os.ftruncate(file.fileno(), 4294967297)
PY
		sparse_file_supported=1
	fi

	ln -s dirA "$FIXTURE/link_dir"
	ln -s does-not-exist "$FIXTURE/broken_link"
	ln -s link_loop_b "$FIXTURE/link_loop_a"
	ln -s link_loop_a "$FIXTURE/link_loop_b"
	for ((level = 1; level <= 40; level++)); do
		chain_next=$((level + 1))
		ln -s "link_chain_$chain_next" "$FIXTURE/link_chain_$level"
	done
	ln -s plain.txt "$FIXTURE/link_chain_41"
	symlink_chain_supported=1
	printf -v long_symlink_target '%04095d' 5
	if ln -s "$long_symlink_target" "$FIXTURE/symlink_target_4095"; then
		max_symlink_supported=1
	fi
	ln -s .. "$FIXTURE/dirA/nested/parent_link"
	ln -s 'file name with spaces.txt' "$FIXTURE/link_with_space"
	printf 'symlink target with spaces\n' >"$FIXTURE/file name with spaces.txt"
	ln "$FIXTURE/plain.txt" "$FIXTURE/hardlink.txt"
	mkfifo "$FIXTURE/test_fifo"

	mkdir "$FIXTURE/deep"
	local_deep_path="$FIXTURE/deep"
	for ((level = 1; level <= 8; level++)); do
		local_deep_path="$local_deep_path/level-$level"
		mkdir "$local_deep_path"
		printf 'depth %d\n' "$level" >"$local_deep_path/file-$level.txt"
	done

	mkdir "$FIXTURE/many"
	for ((index = 1; index <= 512; index++)); do
		printf -v bulk_name 'entry-%03d.dat' "$index"
		printf 'bulk entry %03d\n' "$index" >"$FIXTURE/many/$bulk_name"
	done
	mkdir "$FIXTURE/hardlinks"
	printf 'shared inode\n' >"$FIXTURE/hardlinks/anchor"
	for ((index = 1; index <= 128; index++)); do
		printf -v bulk_name 'link-%03d' "$index"
		ln "$FIXTURE/hardlinks/anchor" "$FIXTURE/hardlinks/$bulk_name"
	done

	printf -v long_component '%0100d' 9
	long_path="$FIXTURE/long_path"
	mkdir "$long_path"
	for ((level = 1; level <= 32; level++)); do
		long_path="$long_path/$long_component"
		mkdir "$long_path"
		printf 'long path depth %d\n' "$level" >"$long_path/file-$level.txt"
	done

	printf 'setuid test\n' >"$FIXTURE/setuid-file"
	printf 'setgid test\n' >"$FIXTURE/setgid-file"
	chmod 4755 "$FIXTURE/setuid-file"
	chmod 2755 "$FIXTURE/setgid-file"
	chmod 1777 "$FIXTURE/dirB"

	if command -v python3 >/dev/null 2>&1; then
		python3 - "$FIXTURE/test_socket" <<'PY'
import socket
import sys

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.bind(sys.argv[1])
sock.close()
PY
	fi
}

if ! prepare_fixture; then
	printf 'No se pudieron preparar los datos temporales de prueba.\n' >&2
	exit 2
fi

printf 'ft_ls mandatory test suite\n'
printf 'Proyecto: %s\n' "$PROJECT_ROOT"
printf 'Referencia: %s\n' "$SYSTEM_LS"
printf 'Locale de comparación: LC_ALL=C, TZ=UTC\n'
printf 'Cada prueba espera Enter. Los resultados FAIL muestran un diff.\n'
printf 'Las salidas se escapan para que nombres con controles no alteren la terminal.\n'
printf 'Todo el fixture se crea bajo %s y se borrará al salir.\n' "$TMP_ROOT"

run_compare 'listado por defecto (sin argumentos)'
run_compare 'opción -a: incluye entradas ocultas' -a
run_compare 'opción -r: invierte el orden alfabético' -r
run_compare 'opción -t: ordena por fecha de modificación' -t
run_compare 'opciones -tr: fecha e inversión combinadas' -tr
run_compare 'opción -l: formato largo y alineación' -l
if [[ "$future_timestamp_supported" -eq 1 ]]; then
	run_compare 'fecha futura cercana en formato largo' -l time-future.txt
else
	run_skip 'fecha futura cercana' 'touch no admite la opción -d'
fi
if [[ "$extreme_timestamps_supported" -eq 1 ]]; then
	run_compare 'límites de fecha: época Unix, 2038 y año 9999' \
		-l time-epoch.txt time-2038.txt time-9999.txt
else
	run_skip 'fechas extremas' 'touch no admite timestamps epoch'
fi
run_compare 'opciones -la: formato largo con ocultos' -la
	if [[ "$nanosecond_sort_supported" -eq 1 ]]; then
		run_compare 'orden por nanosegundos dentro del mismo segundo' \
			-lt nano_times
		run_compare 'orden inverso por nanosegundos dentro del mismo segundo' \
			-ltr nano_times
	else
		run_skip 'orden por nanosegundos' 'Python no está disponible'
	fi
run_compare 'opción -R: recorrido recursivo' -R
run_compare 'opciones -lR: formato largo recursivo' -lR
run_compare 'opciones -laR: recursión con ocultos' -laR
run_compare 'opciones -ltr: formato largo, tiempo e inversión' -ltr
run_compare 'las cinco opciones combinadas' -laRtr
run_compare 'nombres con espacios, tabuladores, saltos y bytes no UTF-8' -a
run_compare 'nombres extraños en formato largo' -la
if [[ "$max_filename_supported" -eq 1 ]]; then
	run_compare 'nombre de archivo con NAME_MAX bytes' -l "$max_name"
	run_compare 'NAME_MAX dentro de un directorio' \
		-l max-name-directory
else
	run_skip 'nombre de NAME_MAX bytes' 'el sistema de archivos no admite 255 bytes'
fi
run_compare 'componente de ruta superior a NAME_MAX' "$oversized_component"
if [[ "$path_too_long_supported" -eq 1 ]]; then
	run_compare 'operando superior a PATH_MAX (camino de solo 4200 bytes)' \
		"$path_too_long"
else
	run_skip 'operando superior a PATH_MAX' \
		'la ruta generada no supera el limite medido'
fi
if [[ "$sparse_file_supported" -eq 1 ]]; then
	run_compare 'archivo disperso de más de 4 GiB en -l' \
		-l sparse-over-4g.bin
else
	run_skip 'archivo disperso superior a 4 GiB' \
		'no hay truncate ni Python para crearlo'
fi
run_compare 'operandos de varios tipos: ficheros y directorios' \
	plain.txt dirA another.txt dirB
run_compare 'operandos repetidos y directorios repetidos' \
	plain.txt plain.txt dirA dirA
run_compare 'rutas explícitas que empiezan por punto (anchos -l)' \
	-l ./dirA/a.txt ./dirA/bigger-name.txt
run_compare 'argumento symlink a directorio sin -l' link_dir
run_compare 'argumento symlink a directorio con -l' -l link_dir
run_compare 'argumento symlink a directorio con -R' -R link_dir
run_compare 'symlink a directorio con barra final' link_dir/
run_compare 'symlinks cíclicos sin recursión infinita' -laR .
run_compare 'symlink roto sin -l' broken_link
run_compare 'symlink roto con -l' -l broken_link
run_compare 'symlink cuyo destino contiene espacios' -l link_with_space
if [[ "$symlink_chain_supported" -eq 1 ]]; then
	run_compare 'cadena de symlinks más larga que el límite del kernel' \
		link_chain_1
	run_compare 'cadena larga de symlinks en formato largo' \
		-l link_chain_1
fi
if [[ "$max_symlink_supported" -eq 1 ]]; then
	run_compare 'symlink con destino de 4095 bytes' -l symlink_target_4095
else
	run_skip 'symlink con destino de 4095 bytes' \
		'el sistema de archivos rechazo el destino'
fi
run_compare 'directorio vacío con formato largo' -l empty
run_compare 'archivo regular de cero bytes con formato largo' -l zero-byte
run_compare 'operando que empieza por guion usando --' -- ./-leading-dash
run_compare 'nombres especiales: hard link, FIFO, socket y bits especiales' \
	-la hardlink.txt test_fifo setuid-file setgid-file dirB
run_compare 'ciento veintinueve enlaces duros al mismo inode' -l hardlinks
run_compare 'enlaces duros con orden reverso' -lr hardlinks
run_compare 'recursión sobre dos directorios y separación de cabeceras' \
	-R dirA dirB
run_compare 'recorrido recursivo con ocho niveles de profundidad' -lR deep
run_compare 'directorio con muchas entradas, orden alfabético inverso' \
	-r many
run_compare 'directorio con muchas entradas, formato largo y orden por tiempo' \
	-ltr many
run_compare 'más de quinientas entradas con reversa y formato largo' -lr many
run_compare 'rutas equivalentes con barras y componentes .' \
	-l dirA//nested/./deep.txt
run_compare 'ruta con más de mil bytes de longitud y recursión larga' \
	-lR long_path
if (ulimit -n 16) 2>/dev/null; then
	run_compare_fd_limit 16 \
		'recursion larga con solo 16 descriptores de archivo disponibles' \
		-lR long_path
else
	run_skip 'límite bajo de descriptores' \
		'el sistema no permite fijar ulimit -n 16'
fi
declare -a missing_operands
missing_operands=()
for ((index = 1; index <= 128; index++)); do
	printf -v missing_name 'missing-%03d' "$index"
	missing_operands+=("$missing_name")
done
missing_operands+=(plain.txt)
run_compare '128 operandos inexistentes y un fichero válido al final' \
	"${missing_operands[@]}"
run_compare 'argumento inexistente junto a un fichero válido' \
	missing-file.txt plain.txt
run_compare 'argumento inexistente junto a un directorio válido' \
	missing-directory dirA

if [[ -e /dev/null && -e /dev/zero ]]; then
	run_compare 'dispositivos del sistema con formato largo' -l /dev/null /dev/zero
else
	run_skip 'dispositivos /dev/null y /dev/zero' \
		'no existen en este sistema'
fi

if [[ "$EUID" -ne 0 ]]; then
	mkdir -p "$FIXTURE/no_search" "$FIXTURE/no_access"
	printf 'entry without search permission\n' >"$FIXTURE/no_search/entry.txt"
	printf 'second entry without search permission\n' \
		>"$FIXTURE/no_search/second-entry.txt"
	mkdir "$FIXTURE/no_search/nested"
	printf 'nested entry behind the permission boundary\n' \
		>"$FIXTURE/no_search/nested/hidden-by-parent.txt"
	printf 'entry without any permission\n' >"$FIXTURE/no_access/entry.txt"
	chmod 444 "$FIXTURE/no_search"
	chmod 000 "$FIXTURE/no_access"
	run_compare 'directorio legible pero sin permiso de búsqueda' no_search
	run_compare 'directorio sin búsqueda en formato largo' -l no_search
	run_compare 'directorio sin búsqueda con -a y -l' -la no_search
	run_compare 'recursión en directorio legible pero sin búsqueda' -R no_search
	run_compare 'recursión larga en directorio legible pero sin búsqueda' \
		-lR no_search
	run_compare 'directorio sin permisos de lectura ni búsqueda' no_access
	chmod 700 "$FIXTURE/no_search" "$FIXTURE/no_access"

	mkdir -p "$FIXTURE/recurse_root/no_access_child"
	printf 'nested protected entry\n' \
		>"$FIXTURE/recurse_root/no_access_child/entry.txt"
	chmod 000 "$FIXTURE/recurse_root/no_access_child"
	run_compare 'recursión con subdirectorio inaccesible' -R recurse_root
	chmod 700 "$FIXTURE/recurse_root/no_access_child"
else
	run_skip 'casos de permisos restringidos' \
		'se ejecuta como root, que puede omitir las restricciones Unix'
fi

printf '\n================ RESUMEN ================\n'
printf 'Pruebas ejecutadas: %d\n' "$total"
printf '%sPASS: %d%s\n' "$C_GREEN" "$passed" "$C_RESET"
printf '%sFAIL: %d%s\n' "$C_RED" "$failed" "$C_RESET"
printf '%sSKIP: %d%s\n' "$C_YELLOW" "$skipped" "$C_RESET"
if [[ "$failed" -eq 0 ]]; then
	printf '%sResultado global: PASS%s\n' "$C_GREEN" "$C_RESET"
	exit 0
fi
printf '%sResultado global: FAIL; revisa los diffs anteriores.%s\n' \
	"$C_RED" "$C_RESET"
exit 1

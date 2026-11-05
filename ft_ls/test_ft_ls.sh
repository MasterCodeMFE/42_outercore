#!/usr/bin/env bash

# ============================================================
# ft_ls mandatory tester
#
# Referencia:
#     LC_ALL=C ls
#
# Uso:
#     ./test_ft_ls.sh
#         Pruebas actualmente implementadas:
#         base, -a, -r, -R
#
#     ./test_ft_ls.sh --all
#         TODO el mandatory:
#         base, -a, -r, -R, -t, -l,
#         combinaciones, errores, memoria y Makefile
# ============================================================

FT_LS="./ft_ls"
MODE="current"

if [ "$1" = "--all" ]; then
	MODE="all"
fi


# ============================================================
# COLORES
# ============================================================

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
CYAN="\033[0;36m"
GRAY="\033[0;90m"
RESET="\033[0m"


# ============================================================
# CONTADORES
# ============================================================

PASSED=0
FAILED=0
SKIPPED=0
TOTAL=0


# ============================================================
# TEMPORALES
# ============================================================

TMP="/tmp/ft_ls_test_$$"

mkdir -p "$TMP"

cleanup()
{
	rm -rf "$TMP"
}

trap cleanup EXIT


# ============================================================
# UTILIDADES
# ============================================================

section()
{
	echo
	printf "${BLUE}============================================================${RESET}\n"
	printf "${BLUE}%s${RESET}\n" "$1"
	printf "${BLUE}============================================================${RESET}\n"
	echo
}


skip_test()
{
	local name="$1"

	printf "${GRAY}[SKIP]${RESET} %s\n" "$name"
	SKIPPED=$((SKIPPED + 1))
}


show_command()
{
	printf "       ${CYAN}Comando:${RESET} $FT_LS"

	for arg in "$@"
	do
		printf " %q" "$arg"
	done

	printf "\n"
}


# ============================================================
# TEST DIFERENCIAL
# ============================================================

run_test()
{
	local name="$1"
	shift

	TOTAL=$((TOTAL + 1))

	LC_ALL=C ls "$@" \
		> "$TMP/ls.out" \
		2> "$TMP/ls.err"

	LS_STATUS=$?

	LC_ALL=C "$FT_LS" "$@" \
		> "$TMP/ft.out" \
		2> "$TMP/ft.err"

	FT_STATUS=$?


	# --------------------------------------------------------
	# COMPARACION
	# --------------------------------------------------------

	if diff -q "$TMP/ls.out" "$TMP/ft.out" > /dev/null \
		&& diff -q "$TMP/ls.err" "$TMP/ft.err" > /dev/null \
		&& [ "$LS_STATUS" -eq "$FT_STATUS" ]
	then
		printf "${GREEN}[OK]${RESET}   %s\n" "$name"
		PASSED=$((PASSED + 1))
		return
	fi


	printf "${RED}[FAIL]${RESET} %s\n" "$name"

	show_command "$@"


	# --------------------------------------------------------
	# STDOUT
	# --------------------------------------------------------

	if ! diff -q "$TMP/ls.out" "$TMP/ft.out" > /dev/null
	then
		echo
		printf "${YELLOW}--- STDOUT ---${RESET}\n"

		diff -u \
			--label "LC_ALL=C ls" \
			--label "LC_ALL=C ft_ls" \
			"$TMP/ls.out" \
			"$TMP/ft.out"
	fi


	# --------------------------------------------------------
	# STDERR
	# --------------------------------------------------------

	if ! diff -q "$TMP/ls.err" "$TMP/ft.err" > /dev/null
	then
		echo
		printf "${YELLOW}--- STDERR ---${RESET}\n"

		diff -u \
			--label "ls stderr" \
			--label "ft_ls stderr" \
			"$TMP/ls.err" \
			"$TMP/ft.err"
	fi


	# --------------------------------------------------------
	# EXIT STATUS
	# --------------------------------------------------------

	if [ "$LS_STATUS" -ne "$FT_STATUS" ]
	then
		echo
		printf "${YELLOW}--- EXIT STATUS ---${RESET}\n"

		echo "ls:    $LS_STATUS"
		echo "ft_ls: $FT_STATUS"
	fi

	echo

	FAILED=$((FAILED + 1))
}


# ============================================================
# COMPROBAR EJECUTABLE
# ============================================================

section "0. PREPARACION"

if [ ! -x "$FT_LS" ]
then
	printf "${RED}[FAIL] ft_ls no existe o no es ejecutable${RESET}\n"
	echo
	echo "Ejecuta:"
	echo
	echo "    make"
	echo
	exit 1
fi

printf "${GREEN}[OK]${RESET}   Existe ./ft_ls\n"


# ============================================================
# 1. MAKEFILE
# ============================================================

section "1. MAKEFILE"

make fclean > "$TMP/make_fclean" 2>&1

if make > "$TMP/make_first" 2>&1
then
	printf "${GREEN}[OK]${RESET}   make compila\n"
	PASSED=$((PASSED + 1))
else
	printf "${RED}[FAIL]${RESET} make falla\n"
	cat "$TMP/make_first"
	FAILED=$((FAILED + 1))
fi


make > "$TMP/make_second" 2>&1

if grep -Eq 'gcc|cc |clang' "$TMP/make_second"
then
	printf "${RED}[FAIL]${RESET} Segundo make recompila\n"
	FAILED=$((FAILED + 1))
else
	printf "${GREEN}[OK]${RESET}   Segundo make no recompila\n"
	PASSED=$((PASSED + 1))
fi


if [ -x "$FT_LS" ]
then
	printf "${GREEN}[OK]${RESET}   Ejecutable se llama ft_ls\n"
	PASSED=$((PASSED + 1))
else
	printf "${RED}[FAIL]${RESET} No se genero ft_ls\n"
	FAILED=$((FAILED + 1))
fi


# ============================================================
# 2. COMPORTAMIENTO BASICO
# ============================================================

section "2. COMPORTAMIENTO BASICO"

run_test "Sin argumentos"
run_test "Directorio actual con ." .
run_test "src" src
run_test "./src" ./src
run_test "src/" src/
run_test "include" include
run_test "obj" obj
run_test "IA" IA


# ============================================================
# 3. MULTIPLES DIRECTORIOS
# ============================================================

section "3. MULTIPLES DIRECTORIOS"

run_test "src include" src include
run_test "include src" include src
run_test "IA src" IA src
run_test "IA src include" IA src include
run_test "src include obj" src include obj


# ============================================================
# 4. -a
# ============================================================

section "4. FLAG -a"

run_test "-a" -a
run_test "-a ." -a .
run_test "-a src" -a src
run_test "-a IA" -a IA
run_test "-a src include" -a src include
run_test "-aa ." -aa .
run_test "-a -a ." -a -a .


# ============================================================
# 5. -r
# ============================================================

section "5. FLAG -r"

run_test "-r" -r
run_test "-r ." -r .
run_test "-r src" -r src
run_test "-r IA" -r IA
run_test "-r src include" -r src include
run_test "-rr ." -rr .


# ============================================================
# 6. -a + -r
# ============================================================

section "6. COMBINACIONES -a -r"

run_test "-ar ." -ar .
run_test "-ra ." -ra .
run_test "-a -r ." -a -r .
run_test "-r -a ." -r -a .
run_test "-ar src" -ar src
run_test "-ar IA" -ar IA
run_test "-ar src include" -ar src include


# ============================================================
# 7. -R
# ============================================================

section "7. FLAG -R"

run_test "-R IA" -R IA
run_test "-R src" -R src
run_test "-R include" -R include
run_test "-R ." -R .
run_test "-RR IA" -RR IA


# ============================================================
# 8. -R COMBINADO
# ============================================================

section "8. COMBINACIONES -R"

run_test "-Ra IA" -Ra IA
run_test "-aR IA" -aR IA

run_test "-Rr IA" -Rr IA
run_test "-rR IA" -rR IA

run_test "-Rar IA" -Rar IA
run_test "-raR IA" -raR IA

run_test "-R IA src" -R IA src
run_test "-Ra IA src" -Ra IA src
run_test "-Rr IA src" -Rr IA src
run_test "-Rar IA src" -Rar IA src


# ============================================================
# 9. FLAGS INTERCALADAS CON RUTAS
# ============================================================

section "9. FLAGS INTERCALADAS"

run_test "src -a" src -a
run_test "src -r" src -r

run_test "src -a include" src -a include
run_test "src -r include" src -r include

run_test "IA -R" IA -R

run_test "IA -a src -R" IA -a src -R
run_test "IA -R src -a" IA -R src -a

run_test "src -a include -r" src -a include -r

run_test \
	"IA -a src -R include -r" \
	IA -a src -R include -r


# ============================================================
# 10. -t
#
# Mandatory, pero solo se ejecuta en --all hasta que lo
# implementes.
# ============================================================

section "10. FLAG -t"

if [ "$MODE" = "all" ]
then

	run_test "-t" -t
	run_test "-t ." -t .
	run_test "-t src" -t src
	run_test "-ta ." -ta .
	run_test "-tr ." -tr .
	run_test "-tar ." -tar .

	run_test "-t src include" -t src include

	run_test "-Rt IA" -Rt IA
	run_test "-Rtr IA" -Rtr IA
	run_test "-Rta IA" -Rta IA
	run_test "-Rtar IA" -Rtar IA

else

	skip_test "-t"
	skip_test "-ta"
	skip_test "-tr"
	skip_test "-tar"
	skip_test "-Rt"
	skip_test "-Rtr"

fi


# ============================================================
# 11. -l
# ============================================================

section "11. FLAG -l"

if [ "$MODE" = "all" ]
then

	run_test "-l" -l
	run_test "-l ." -l .
	run_test "-l src" -l src

	run_test "-la ." -la .
	run_test "-lr ." -lr .
	run_test "-lt ." -lt .

	run_test "-lar ." -lar .
	run_test "-lat ." -lat .
	run_test "-ltr ." -ltr .
	run_test "-latr ." -latr .

	run_test "-l src include" -l src include

	run_test "-lR IA" -lR IA
	run_test "-laR IA" -laR IA
	run_test "-lRr IA" -lRr IA
	run_test "-lRt IA" -lRt IA
	run_test "-laRt IA" -laRt IA
	run_test "-laRtr IA" -laRtr IA

else

	skip_test "-l"
	skip_test "-la"
	skip_test "-lr"
	skip_test "-lt"
	skip_test "-latr"
	skip_test "-lR"
	skip_test "-laRt"

fi


# ============================================================
# 12. COMBINACIONES COMPLETAS MANDATORY
# ============================================================

section "12. COMBINACIONES COMPLETAS"

if [ "$MODE" = "all" ]
then

	run_test "-lRart ." -lRart .
	run_test "-lRat ." -lRat .
	run_test "-lRtr ." -lRtr .
	run_test "-laRtr ." -laRtr .

	run_test "-laRtr IA" -laRtr IA
	run_test "-laRtr IA src" -laRtr IA src

	run_test \
		"flags separadas completas" \
		-l -R -a -r -t IA

else

	skip_test "-lRart"
	skip_test "-laRtr IA"
	skip_test "Todas las flags separadas"

fi


# ============================================================
# 13. ARCHIVOS COMO ARGUMENTO
# ============================================================

section "13. ARCHIVOS COMO ARGUMENTO"

if [ "$MODE" = "all" ]
then

	run_test "Makefile" Makefile
	run_test "README.md" README.md

	run_test "-l Makefile" -l Makefile
	run_test "-a Makefile" -a Makefile

	run_test \
		"archivo + directorio" \
		Makefile src

	run_test \
		"directorio + archivo" \
		src Makefile

	run_test \
		"varios archivos" \
		Makefile README.md en.subject.pdf

	run_test \
		"archivos + directorios" \
		Makefile src README.md include

else

	skip_test "Archivo individual"
	skip_test "Archivo + directorio"
	skip_test "Varios archivos"

fi


# ============================================================
# 14. ERRORES
# ============================================================

section "14. ERRORES"

if [ "$MODE" = "all" ]
then

	run_test \
		"Ruta inexistente" \
		__ft_ls_NO_EXISTE__

	run_test \
		"Ruta valida + ruta inexistente" \
		src __ft_ls_NO_EXISTE__

	run_test \
		"Opcion invalida" \
		-Z

else

	skip_test "Ruta inexistente"
	skip_test "Opcion invalida"

fi


# ============================================================
# 15. DATASET ESPECIAL
# ============================================================

section "15. DATASET ESPECIAL"

DATA="$TMP/dataset"

mkdir -p "$DATA"
mkdir -p "$DATA/dir_a"
mkdir -p "$DATA/dir_b"
mkdir -p "$DATA/.hidden_dir"
mkdir -p "$DATA/deep/level1/level2"

touch "$DATA/a"
touch "$DATA/b"
touch "$DATA/z"
touch "$DATA/A"
touch "$DATA/B"

touch "$DATA/.hidden"
touch "$DATA/.another_hidden"

touch "$DATA/space file"
touch "$DATA/very_long_filename_abcdefghijklmnopqrstuvwxyz"

touch "$DATA/dir_a/file_a"
touch "$DATA/dir_b/file_b"
touch "$DATA/.hidden_dir/secret"

touch "$DATA/deep/root"
touch "$DATA/deep/level1/file1"
touch "$DATA/deep/level1/level2/file2"

ln -s "a" "$DATA/link_to_a"
ln -s "dir_a" "$DATA/link_to_dir"
ln -s "does_not_exist" "$DATA/broken_link"

mkfifo "$DATA/test_fifo"


run_test "Dataset normal" "$DATA"
run_test "Dataset -a" -a "$DATA"
run_test "Dataset -r" -r "$DATA"
run_test "Dataset -ar" -ar "$DATA"

run_test "Dataset -R" -R "$DATA"
run_test "Dataset -Ra" -Ra "$DATA"
run_test "Dataset -Rr" -Rr "$DATA"
run_test "Dataset -Rar" -Rar "$DATA"


if [ "$MODE" = "all" ]
then

	run_test "Dataset -t" -t "$DATA"
	run_test "Dataset -tr" -tr "$DATA"

	run_test "Dataset -l" -l "$DATA"
	run_test "Dataset -la" -la "$DATA"
	run_test "Dataset -lr" -lr "$DATA"
	run_test "Dataset -lt" -lt "$DATA"

	run_test "Dataset -lR" -lR "$DATA"
	run_test "Dataset -laR" -laR "$DATA"
	run_test "Dataset -laRtr" -laRtr "$DATA"

	run_test "Symlink" "$DATA/link_to_a"
	run_test "Symlink a directorio" "$DATA/link_to_dir"
	run_test "Symlink roto" "$DATA/broken_link"

	run_test "FIFO con -l" -l "$DATA/test_fifo"

fi


# ============================================================
# 16. FECHAS PARA -t
# ============================================================

section "16. ORDENACION TEMPORAL"

if [ "$MODE" = "all" ]
then

	TIME="$TMP/time_dataset"

	mkdir -p "$TIME"

	touch "$TIME/old"
	sleep 1

	touch "$TIME/middle"
	sleep 1

	touch "$TIME/new"

	run_test "-t fechas" -t "$TIME"
	run_test "-tr fechas" -tr "$TIME"
	run_test "-rt fechas" -rt "$TIME"

	run_test "-lt fechas" -lt "$TIME"
	run_test "-ltr fechas" -ltr "$TIME"

else

	skip_test "Dataset temporal -t"
	skip_test "Dataset temporal -tr"

fi


# ============================================================
# 17. PERMISOS PARA -l
# ============================================================

section "17. PERMISOS -l"

if [ "$MODE" = "all" ]
then

	PERM="$TMP/permissions"

	mkdir -p "$PERM"

	touch "$PERM/normal"
	touch "$PERM/executable"
	touch "$PERM/no_permissions"

	chmod 644 "$PERM/normal"
	chmod 755 "$PERM/executable"
	chmod 000 "$PERM/no_permissions"

	run_test "Permisos -l" -l "$PERM"
	run_test "Permisos -la" -la "$PERM"

	chmod 644 "$PERM/no_permissions"

else

	skip_test "Permisos -l"

fi


# ============================================================
# 18. MEMORY LEAKS
# ============================================================

section "18. VALGRIND"

if [ "$MODE" = "all" ]
then

	if command -v valgrind > /dev/null 2>&1
	then

		valgrind \
			--leak-check=full \
			--show-leak-kinds=all \
			--errors-for-leak-kinds=definite,indirect \
			--error-exitcode=42 \
			"$FT_LS" -R "$DATA" \
			> "$TMP/valgrind.out" \
			2> "$TMP/valgrind.err"

		STATUS=$?

		if [ "$STATUS" -eq 0 ]
		then
			printf "${GREEN}[OK]${RESET}   Valgrind -R dataset\n"
			PASSED=$((PASSED + 1))
		else
			printf "${RED}[FAIL]${RESET} Valgrind ha detectado problemas\n"
			grep -E \
				'definitely lost|indirectly lost|ERROR SUMMARY' \
				"$TMP/valgrind.err"

			FAILED=$((FAILED + 1))
		fi

	else

		printf "${YELLOW}[SKIP]${RESET} Valgrind no esta instalado\n"
		SKIPPED=$((SKIPPED + 1))

	fi

else

	skip_test "Valgrind completo"

fi


# ============================================================
# 19. RESUMEN
# ============================================================

section "RESULTADO FINAL"

printf "Modo:      %s\n" "$MODE"
printf "Ejecutados: %d\n" "$TOTAL"

printf "${GREEN}OK:         %d${RESET}\n" "$PASSED"
printf "${RED}Fallos:     %d${RESET}\n" "$FAILED"
printf "${YELLOW}Saltados:   %d${RESET}\n" "$SKIPPED"

echo

if [ "$FAILED" -eq 0 ]
then

	if [ "$MODE" = "all" ]
	then
		printf "${GREEN}"
		echo "TODOS LOS TESTS DEL MANDATORY HAN PASADO"
		printf "${RESET}"
	else
		printf "${GREEN}"
		echo "TODO LO IMPLEMENTADO ACTUALMENTE HA PASADO"
		printf "${RESET}"
		echo
		echo "Para ejecutar TODO el mandatory:"
		echo
		echo "    ./test_ft_ls.sh --all"
	fi

	exit 0
fi

printf "${RED}"
echo "HAY TESTS QUE NO COINCIDEN CON LC_ALL=C ls"
printf "${RESET}"

exit 1
#! /bin/bash

podir=po
files=`find . -name Messages.sh`
dirs=`for i in $files; do echo \`dirname $i\`; done | sort -u`
tmpname="$PWD/messages.log"
EXTRACTRC=${EXTRACTRC:-extractrc}
EXTRACTATTR=${EXTRACTATTR:-extractattr}
EXTRACTQML=${EXTRACTQML:-extractqml}
PREPARETIPS=${PREPARETIPS:-preparetips}
REPACKPOT=${REPACKPOT:-repack-pot.pl}
export EXTRACTRC EXTRACTATTR EXTRACTQML PREPARETIPS REPACKPOT
if [ "x$IGNORE" = "x" ]; then
	IGNORE="/tests/"
else
	IGNORE="$IGNORE
/tests/"
fi

for subdir in $dirs; do
  # skip Messages.sh files of KDevelop's app templates
  grep '{APPNAMELC}[^ ]*.pot' $subdir/Messages.sh 1>/dev/null && continue

  test -z "$VERBOSE" || echo "Making messages in $subdir"
  (cd $subdir
   if find . -name \*.c\* -o -name \*.h\* | fgrep -v "$IGNORE" | xargs fgrep -s -q KAboutData ; then
	echo 'i18nc("NAME OF TRANSLATORS","Your names");' >> rc.cpp
	echo 'i18nc("EMAIL OF TRANSLATORS","Your emails");' >> rc.cpp
   fi

   # using xgettext 0.15 or later
   ### TODO what --flags param should be used?
   XGETTEXT_FLAGS_QT="--from-code=UTF-8 -C --qt -ktr:1,1t -ktr:1,2c,2t -kQT_TRANSLATE_NOOP:1c,2,2t -kQT_TR_NOOP:1,1t -ktranslate:1c,2,2t -ktranslate:2,3c,3t"
   XGETTEXT_FLAGS="--copyright-holder=This_file_is_part_of_KDE --from-code=UTF-8 -C --kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kI18N_NOOP2_NOSTRIP:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 --msgid-bugs-address=http://bugs.kde.org"
   export XGETTEXT_FLAGS
   export XGETTEXT_FLAGS_QT

   if test -f Messages.sh; then
       # Note: Messages.sh is supposed to get the translators' placeholder by rc.cpp
       podir=$podir srcdir=. XGETTEXT_PROGRAM="${XGETTEXT:-xgettext}" XGETTEXT_QT="${XGETTEXT:-xgettext} $XGETTEXT_FLAGS_QT" XGETTEXT="${XGETTEXT:-xgettext} $XGETTEXT_FLAGS" bash Messages.sh
   fi
   exit_code=$?
   if test "$exit_code" -ne 0; then
       echo "Bash exit code: $exit_code"
   else
       rm -f rc.cpp
   fi
   ) >& $tmpname
   test -s $tmpname && { echo $subdir ; cat "$tmpname"; }
done

# # Repack extracted templates.
# for potfile in $podir/*.pot; do
#     $REPACKPOT $potfile
# done

rm -f $tmpname

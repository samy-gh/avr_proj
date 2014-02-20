" vim:set ts=4 sts=0 tw=0 fdm=marker: (この行に関しては:help modelineを参照)
"
" vim初期化ファイルサンプル
"
" Last Change: 20-Feb-2014.
" foldの操作方法を知らない場合はとりあえず "zR" とタイプ
"

scriptencoding utf-8

" このスクリプトの親ディレクトリを取得
if has( 'win32' )
	let s:s_curdir = expand( '<sfile>:h' ) . '\'
else
	let s:s_curdir = expand( '<sfile>:h' ) . '/'
endif
"echo s:s_curdir

if &filetype == 'c'
	call AddTagsL( s:s_curdir . 'tags' )
	call AddPathL( s:s_curdir . 'samylib' )

	call AddTagsGlobalToLocal()
	call AddPathGlobalToLocal()

	" includeのtags作成コマンド
	" ctags -R --tag-relative=yes --C-kinds=+p .
endif

if has( 'win32' )
	" MinGW/cygwin (gcc)用efm設定
	let s:as_efm = []
	let s:as_efm = add( s:as_efm, "%Dmake: Entering directory '%f',%Dmake: Leaving directory '%f'" )

	" エラーメッセージ(複数行、日本語)
	let s:as_efm = add( s:as_efm, '%W%f:%l:%c: 警告: %m' )
	let s:as_efm = add( s:as_efm, '%E%f:%l:%c: エラ%.: %m' )	" エラーの「ー」は'['を含む
	let s:as_efm = add( s:as_efm, '%I%f:%l:%c: 備考: %m' )

	" エラーメッセージ(複数行、英語)
	let s:as_efm = add( s:as_efm, '%W%f:%l:%c: warning: %m' )
	let s:as_efm = add( s:as_efm, '%E%f:%l:%c: error: %m' )
	let s:as_efm = add( s:as_efm, '%I%f:%l:%c: note: %m' )

	" MinGW/cygwin共通
	let s:as_efm = add( s:as_efm, '%A%f:%l:%c:%m' )	" warning, error, note以外
	let s:as_efm = add( s:as_efm, '%m %f:%l:%c:' )
	let s:as_efm = add( s:as_efm, '%f:%l: %m' )		" 桁番号が無いエラー用

	" 複数行の2行目以降
	let s:as_efm = add( s:as_efm, '%-Z [ 	]%#^' )
	let s:as_efm = add( s:as_efm, '%-C %.%#' )

	let &efm = join( s:as_efm, ',' )
endif


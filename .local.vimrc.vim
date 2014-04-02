" vim:set ts=4 sts=0 tw=0 fdm=marker: (この行に関しては:help modelineを参照)
"
" vim初期化ファイルサンプル
"
" Last Change: 2014/04/02 20:20.
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

if (&filetype == 'c') || (&filetype == 'h')
	call AddTagsL( s:s_curdir . 'tags' )
	call AddPathL( s:s_curdir . 'samylib' )
	call AddPathL( 'C:\Program Files\WinAVR-20100110\avr\include' )

	call AddTagsGlobalToLocal()
	call AddPathGlobalToLocal()

	" includeのtags作成コマンド
	" ctags -R --tag-relative=yes --C-kinds=+p .
endif

if has( 'win32' )
	" errorformat設定
	call errorformat#set_gcc()
endif


" vim:set ts=4 sts=0 tw=0 fdm=marker: (この行に関しては:help modelineを参照)
"
" vim初期化ファイルサンプル
"
" Last Change: 2014/04/05 15:54.
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
	call AddPathL( s:s_curdir )
	call AddPathL( s:s_curdir . 'usbdrv' )

	" includeのtags作成コマンド
	" ctags -R --tag-relative=yes --C-kinds=+p .
endif

if !exists( 'g:did_local_vimrc_make' )
	let g:did_local_vimrc_make = 1
	if has( 'win32' )
		" 指定パスでmakeを実施する様に設定(cygwin_vim_makeprg.batにお任せ)
		let &makeprg = s:s_curdir . 'bat_make.bat ' . s:s_curdir

		" s:make_dirにあるget_log.shコマンドを実行して結果をQuickFixへ表示
		" 「Run」コマンドの引数を省略した場合は最後の値を使用。
		command! -nargs=* Run
			\   if '<args>' != ''
			\ | 	let s:makeprg_args = '<args>'
			\ | elseif !exists( 's:makeprg_args' )
			\ | 	let s:makeprg_args = ''
			\ | endif
			\ | let s:makeprg_bak = &makeprg
			\ | let &makeprg = s:s_curdir . 'w.bat'
			\ | make
			\ | let &makeprg = s:makeprg_bak
			\ | if has( 'gui' )
			\ | 	echo 'finish'
			\ | endif
	else
		if executable( 'gmake' )
			let &makeprg = 'gmake -j2 -s -w -C ' . s:s_curdir
		else
			let &makeprg = 'make -j2 -s -w -C ' . s:s_curdir
		endif
	endif
endif


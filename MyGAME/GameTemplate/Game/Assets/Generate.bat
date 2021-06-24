@echo off
rem 変数の即時展開をオフ
setlocal enabledelayedexpansion

echo 収集したいファイルの拡張子を指定してください。
set /p extension=→

rem 出力先ファイルパスの設定
set outputpath=!extension!Assets.h

cd ..

findstr pragma %outputpath% > nul
if %ERRORLEVEL%==1 (
	echo よって、ファイルを新規作成します。
	echo #pragma once > %outputpath%
	echo. >> %outputpath%
	echo. >> %outputpath%
)

cd Assets

rem .tkmとついたファイルを検索
for /f "usebackq" %%a in (`dir /s /b *.!extension!`) do (
	call :DoExtract %%a
)

echo 終了しました。

pause

exit

:DoExtract
		rem Gameまで移動
		cd ..
		rem Gameまでのファイルパスを取得
		set directorypath=%cd%\
		rem Assetsに戻る
		cd Assets
		rem モデルのパス格納
		set filepath=%1
		rem モデルのパスからGame\までのパスを削除
		set filepath=!filepath:%directorypath%=!
		
		rem \から/に変換
		set filepath=!filepath:\=/!
		
		rem 検索のためResult.txtがある場所に行く
		cd ..
		rem 既に書き込まれているか検索 .を入れると正規表現になるので拡張子の4文字を除いて検索
		findstr !filepath:~0,-4! %outputpath% > nul
		
		rem 書き込まれてなかったら
		if %ERRORLEVEL%==1 (
			:preName
			rem パスにつける名前を入力
			echo !filepath!　につける名前を入力してください。
			set /p name=→
			findstr !name! %outputpath% > nul
			if !ERRORLEVEL!==1 (
			rem 書き込み
			echo const char* !name! = L"!filepath!"; >> %outputpath%
			) else (
				echo 既に使われているパスです。
				goto preName
			)
		)
		
		rem Result.txtがあった場所からAssetsまで戻ってくる
		cd Assets
		rem サブルーチンから抜ける
		exit /b
		
		
		
		
		
		rem made by Junou Nishikiori
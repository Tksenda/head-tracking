int fd1[2], fd2[2]; // 双方向に通信を行うため，パイプを2本生成する  

/* パイプ生成 */
pipe(fd1);
pipe(fd2); 

if((pid = fork()) == 0) {
  close(fd1[1]); // 入力端を閉じる
  close(fd2[0]); // 出力端を閉じる

  /* fd1[0]を読み込み，fd2[1]を書き込みに利用する */

  close(fd1[0]);
  close(fd2[1]);
  exit(0);
}
else if(pid >= 1) {
  close(fd1[0]); // 出力端を閉じる
  close(fd2[1]); // 入力端を閉じる

  /* fd1[1]を書き込み，fd2[0]を読み込みに利用する */

  close(fd1[1]);
  close(fd2[0]);
  wait(0);
}

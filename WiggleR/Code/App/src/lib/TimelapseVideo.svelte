<script lang="ts">
	import { start, end, showThresh, showContours } from '../store/store';

	let startTime = '';
	let endTime = '';
	let threshActive: null | boolean = false;
	let contoursActive: null | boolean = false;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
	showThresh.subscribe((show) => (threshActive = Boolean(show)));
	showContours.subscribe((show) => (contoursActive = Boolean(show)));

	function onChangeThresh(e: { target: { active: boolean } }) {
		showThresh.update((show) => (show = e.target.active));
	}
	function onChangeContours(e: { target: { active: boolean } }) {
		showContours.update((show) => (show = e.target.active));
	}
</script>

<div>
	{#key [startTime, endTime, threshActive, contoursActive]}
		<video width="1024" height="768" controls>
			<source
				src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}&showThresh=${threshActive}&showContours=${contoursActive}`}
				type="video/mp4"
			/>
		</video>
	{/key}
	<!-- <div class="video-settings">
		<kor-toggle
			grid-cols="2"
			label="Thresh"
			on:active-changed={onChangeThresh}
			active={threshActive ? true : null}
		/>
		<kor-toggle
			grid-cols="2"
			label="Contours"
			on:active-changed={onChangeContours}
			active={contoursActive ? true : null}
		/>
	</div> -->
</div>

<style>
	video {
		border: 10px solid var(--main-color-darker);
	}
	.video-settings {
		display: flex;
		gap: 10px;
	}
</style>
